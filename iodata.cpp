#include "iodata.hpp"

void generateUsers() {
    int n;
    std::cout << "User count: ";
    std::cin >> n;

    std::mt19937 mt(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> randLength(3, 10), 
                                       randConsonent(0, 19), 
                                       randVowel(0, 5), 
                                       randOddEven(0, 1), 
                                       randBalance(100, 1000000);

    char consonents[] = {'b','c','d','f','g','h','j','k','l','m','n','p','q','r','s','t','v','w','x','z'};
    char vowels[] = {'a','e','i','o','u','y'};

    std::ofstream out ("users.txt");
    std::string name;
    int length;
    bool isOdd;
    std::ostringstream sstream;
    for (int i = 0; i < n; i ++) {
        name = "";
        length = randLength(mt);
        isOdd = randOddEven(mt);
        
        // Name generation
        for (int j = 0; j < length; j ++) {
            if (isOdd)
                name += consonents[randConsonent(mt)];
            else name += vowels[randVowel(mt)];
            isOdd = !isOdd;
        }

        // Seed for primary key
        sstream.str("");
        sstream.clear();
        sstream << name << i;

        out << name << " " << hash(sstream.str()) << " " << randBalance(mt) << "\n";
    }
    out.close();
    std::cout << "\nusers.txt file has been generated\n";
}

std::vector<User> getUsers() {
    std::vector<User> users;
    User temp;
    std::ifstream in ("users.txt");
    while (in >> temp.name) {
        in >> temp.publicKey >> temp.balance;
        users.push_back(temp);
    }
    in.close();
    return users;
}

void generateTransactions() {
    int t;
    std::cout << "Transaction count: ";
    std::cin >> t;

    std::vector<User> users = getUsers();

    std::mt19937 mt(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> randUser(0, users.size()-1), randAmount(1, 1000000);

    User sender, receiver;
    double amount;
    int senderIndex;
    std::ofstream out ("transactions.txt");
    for (int i = 0; i < t; i ++) {
        // Get a random seeder that has >= 0.01 balance
        do {
            senderIndex = randUser(mt);
            sender = users[senderIndex];
        } while (sender.balance < 0.01);

        // Find a random receiver that isn't the same person
        do {
            receiver = users[randUser(mt)];
        } while (sender.publicKey == receiver.publicKey);

        // Set a random amount of money
        if (sender.balance > 1) {
            amount = randAmount(mt);
            if (amount > sender.balance) {
                amount = sender.balance * amount / 1000000.0;
                if (amount < 1)
                    amount = 1;
            }
        } else amount = sender.balance;
        
        // Subtract the amount from the sender for further transactions
        users[senderIndex].balance -= amount;

        // Generate transaction key
        Transaction T(sender.publicKey, receiver.publicKey, amount);

        out << T.ID << " " << T.senderKey << " " << T.receiverKey << " " << T.amount << "\n";
    }
    out.close();

    std::cout << "\ntransactions.txt file has been generated\n";
}

int getUserIndexByKey(std::vector<User> &users, std::string key) {
    int index = 0;
    while(key != users[index].publicKey)
        index ++;
    return index;
}

bool verifyBalance(std::vector<User> &users, Transaction t) {
    int userIndex = getUserIndexByKey(users, t.senderKey);
    if(users[userIndex].balance >= t.amount) {
        users[userIndex].balance -= t.amount;
        return true;
    } else {
        return false;
    }
}

bool verifyTransHash(Transaction t) {
    std::ostringstream body;
    body << t.senderKey << t.receiverKey << t.amount;
    std::string bodyHash = hash(body.str());
    if (bodyHash == t.ID)
        return true;
    else return false;
}

std::vector<Transaction> getTransactions() {
    std::vector<Transaction> trans;
    Transaction temp;
    std::ifstream in ("transactions.txt");
    while (in >> temp.ID) {
        in >> temp.senderKey >> temp.receiverKey >> temp.amount;
        trans.push_back(temp);
    }
    in.close();
    return trans;
}

std::vector<Transaction> getNTransactions(int n) {
    std::vector<Transaction> trans = getTransactions();
    int initialTransSize = trans.size();
    if (initialTransSize == 0)
        return trans;

    std::vector<User> users = getUsers();
    std::vector<Transaction> selected;
    std::mt19937 mt(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> randTrans(0, initialTransSize - 1);    // random transaction index
    int index;
    bool skipIndex;

    for (int i = 0; i < n; i ++) {
        do {
            skipIndex = false;

            // get a random index of a transaction
            index = randTrans(mt);
            if (index > trans.size()-1) {
                index = (trans.size() - 1) * index / (initialTransSize - 1);
            }

            // if the sender doesn't have enough money for the transaction or the transactions's 
            // hash doesn't match its ID, erase the transaction and generate another index
            if (!verifyBalance(users, trans[index]) || !verifyTransHash(trans[index])) {
                trans.erase(trans.begin()+index);
                skipIndex = true;
            }
        } while (skipIndex);

        selected.push_back(trans[index]);
        trans.erase(trans.begin()+index);

        if(trans.size() == 0) {
            break;
        }
    }
    return selected;
} 

void removeTransactions(std::vector<Transaction> &used) {
    std::vector<Transaction> all = getTransactions();

    for (int i = 0; i < used.size(); i ++) {
        auto position = std::find(all.begin(), all.end(), used[i]);
        if (position != all.end()) // if the element was found
            all.erase(position);
    }

    std::ofstream out ("transactions.txt");
    for (int i = 0; i < all.size(); i ++) {
        out << all[i].ID << " " << all[i].senderKey << " "
            << all[i].receiverKey << " " << all[i].amount << "\n";
    }
    out.close();
}
