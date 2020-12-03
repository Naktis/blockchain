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

// export PKG_CONFIG_PATH=/home/daisy/libitcoinas/lib/pkgconfig
// g++ -std=c++11 -o main merkle.cpp hash.cpp iodata.cpp blockchain.cpp main.cpp $(pkg-config --cflags --libs libbitcoin)