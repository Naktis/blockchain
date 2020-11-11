#pragma once

#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include "hash.hpp"

class User {
    public:
        std::string name;
        std::string publicKey;
        double balance;
};

class Transaction {
    public:
        Transaction() {}
        Transaction(std::string senderKey, std::string receiverKey, double amount) {
            this->senderKey = senderKey;
            this->receiverKey = receiverKey;
            this->amount = amount;

            std::ostringstream fieldValues;
            fieldValues << senderKey << receiverKey << amount;
            ID = hash(fieldValues.str());
        }

        std::string ID; // hash of other field values
        std::string senderKey;
        std::string receiverKey;
        double amount;
};

void generateUsers();
std::vector<User> getUsers();
void generateTransactions();
int getUserIndexByKey(std::vector<User> &users, std::string key);
bool verifyBalance(std::vector<User> &users, Transaction t);
bool verifyTransHash(Transaction t);
std::vector<Transaction> getTransactions();
std::vector<Transaction> getNTransactions(int n);