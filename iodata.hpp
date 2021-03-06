#pragma once

#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <algorithm>
#include "hash.hpp"

class User {
    public:
        std::string name;
        std::string publicKey;
        double balance;
};

class Transaction {
    public:
        std::string ID; // hash of other field values
        std::string senderKey;
        std::string receiverKey;
        double amount;

        Transaction() {}
        Transaction(std::string senderKey, std::string receiverKey, double amount) {
            this->senderKey = senderKey;
            this->receiverKey = receiverKey;
            this->amount = amount;

            std::ostringstream fieldValues;
            fieldValues << senderKey << receiverKey << amount;
            ID = hash(fieldValues.str());
        }
        inline bool operator==(const Transaction& another){ return this->ID == another.ID;}
};

void generateUsers();
std::vector<User> getUsers();
void printUsers(std::vector<User> &users);
int getUserIndexByKey(std::vector<User> &users, std::string key);
bool verifyBalance(std::vector<User> &users, Transaction t);

void generateTransactions();
std::vector<Transaction> getTransactions();
std::vector<Transaction> getNTransactions(int n);
void removeTransactions(std::vector<Transaction> &used);
bool verifyTransHash(Transaction t);