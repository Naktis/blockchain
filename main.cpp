#include "blockchain.hpp"

int main() {
    int fun;
    do {
        std::cout << "\nEnter the function number:\n"
              << "1 - User generation\n"
              << "2 - Transaction generation\n"
              << "3 - Blockchain mining\n"
              << "0 - Exit\n";
    
        std::cin >> fun;
        switch (fun) {
            case 1: 
                generateUsers();
                break;
            case 2: 
                generateTransactions();
                break;
            case 3: 
                createWholeChain();
                break;
            case 0: 
                break;
            default:
                std::cout << "Invalid input";
        }
    } while (fun != 0);
}

// g++ -c blockchain.cpp hash.cpp iodata.cpp main.cpp
// g++ -o g blockchain.o hash.o iodata.o main.o