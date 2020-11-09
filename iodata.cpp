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

void generateTransactions() {
    int t;
    std::cout << "Transaction count: ";
    std::cin >> t;

    // Read users
    std::vector<User> users;
    User temp;
    std::ifstream in ("users.txt");
    while (in >> temp.name) {
        in >> temp.publicKey >> temp.balance;
        users.push_back(temp);
    }
    in.close();

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

std::vector<Transaction> getTransactions(int n) {
    std::vector<Transaction> trans;
    Transaction temp;
    std::ifstream in ("transactions.txt");
    while (in >> temp.ID) {
        in >> temp.senderKey >> temp.receiverKey >> temp.amount;
        trans.push_back(temp);
    }
    in.close();

    std::mt19937 mt(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> randTrans(0, trans.size()-1);

    std::vector<Transaction> selected;
    std::vector<int> indexes;
    int index;
    int alreadyUsed;
    for (int i = 0; i < n; i ++) {
        do {
            alreadyUsed = false;
            index = randTrans(mt);
            for (int j = 0; j < indexes.size(); j++) {
                if (index == indexes[j]) {
                    alreadyUsed = true;
                    break;
                }
            }
        } while (alreadyUsed);

        indexes.push_back(index);
        selected.push_back(trans[index]);
    }

    std::ofstream out ("transactions.txt");

    for (int i = 0; i < trans.size(); i ++) {
        alreadyUsed = false;
        for (int j = 0; j < indexes.size(); j++) {
                if (i == indexes[j]) {
                    alreadyUsed = true;
                    break;
                }
        }
        if (!alreadyUsed)
            out << trans[i].ID << " " << trans[i].senderKey << " "
                << trans[i].receiverKey << " " << trans[i].amount << "\n";
    }
    out.close();
    return selected;
} 