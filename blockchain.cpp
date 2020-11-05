#include <fstream>
#include <random>
#include "block.hpp"

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
    std::cout << "\nusers.txt file has been generated";
}

void generateTransactions() {
    int t, u;
    std::cout << "Transaction count: ";
    std::cin >> t;
    std::cout << "User count: ";
    std::cin >> u;

    // Reading users
    std::vector<User> users;
    User temp;
    std::ifstream in ("users.txt");
    for (int i = 0; i < u; i ++) {
        in >> temp.name >> temp.publicKey >> temp.balance;
        users.push_back(temp);
    }
    in.close();

    std::mt19937 mt(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> randUser(0, u-1), 
                                       randAmount(1, 1000000);

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
    std::cout << "\ntransactions.txt file has been generated";
}

int main() {
    std::cout << "Enter the function number:\n"
              << "1 - User generation\n"
              << "2 - Transaction\n";

    int fun;
    std::cin >> fun;
    switch (fun) {
        case 1: 
            generateUsers();
            break;
        case 2: 
            generateTransactions();
            break;
        default:
            std::cout << "Invalid input";
    }
}

// g++ -c blockchain.cpp hash.cpp
// g++ -o g blockchain.o hash.o