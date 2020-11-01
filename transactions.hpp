#pragma once

#include "hash.hpp"

class User {
    public:
        std::string name;
        std::string publicKey;
        double balance;
};

class Transaction {
    public:
        Transaction(std::string senderKey, std::string receiverKey, double amount) {
            this->senderKey = senderKey;
            this->receiverKey = receiverKey;
            this->amount = amount;

            std::stringstream fieldValues;
            fieldValues << senderKey << receiverKey << amount;
            ID = hash(fieldValues.str());
            std::cout << fieldValues.str() << " " << ID << "\n";
        }

    private:
        std::string ID; // hash of other field values
        std::string senderKey;
        std::string receiverKey;
        double amount;
};