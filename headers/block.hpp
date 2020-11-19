#pragma once
#pragma warning(disable:4996)

#include "iodata.hpp"

class Block {
public:
    Block(std::string prevHash, std::vector<Transaction> body) {
        this->prevHash = prevHash;
        this->body = body;
        this->merkleRootHash = transMerkleRoot(body);
    }

    std::string Timestamp() {
        std::stringstream ss;
        ss << std::put_time(localtime(&timestamp), "%Y-%m-%d %X");
        return ss.str();
    }

    std::string HeaderHash() {
        std::ostringstream sstream;
        sstream << prevHash << timestamp << version << merkleRootHash << nonce << difficulty;
        return hash(sstream.str());
    }

    bool isNonceValid(int nonce) {
        this->nonce = nonce;
        this->timestamp = currentTime();

        std::string hash = HeaderHash();
        std::string zeros(difficulty, '0');
        if (hash.substr(0, difficulty) == zeros)
            return true;
        else return false;
    }

    int Nonce() { return nonce; }
    std::vector<Transaction> Body() { return body; }

private:
    std::string prevHash;
    time_t timestamp;
    double version = 1;
    std::string merkleRootHash;
    int nonce;
    int difficulty = 3;
    std::vector<Transaction> body;

    time_t currentTime() {
        auto now = std::chrono::system_clock::now();
        return std::chrono::system_clock::to_time_t(now);
    }

    std::string merkleRoot(std::vector<std::string> incoming) {
        if (incoming.size() != 1) {                 // continue until the root hash is left
            std::vector<std::string> outcoming;

            if (incoming.size() % 2 != 0)           // if the num of transactions is odd
                incoming.push_back(incoming.back());// add the duplicate of the last transaction

            for (int i = 0; i < incoming.size(); i = i + 2)
                outcoming.push_back(hash(incoming[i] + incoming[i + 1])); // hash pairs

            return(merkleRoot(outcoming));
        }
        else {
            return incoming.front();
        }
    }

    std::string transMerkleRoot(std::vector<Transaction> t) {
        if (t.empty())
            return hash("");

        // make a vector that holds all transactions' IDs
        std::vector<std::string> allTransHashes;
        for (int i = 0; i < t.size(); i++)
            allTransHashes.push_back(t[i].ID);

        return merkleRoot(allTransHashes);
    }
};