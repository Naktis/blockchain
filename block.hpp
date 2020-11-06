#pragma once

#include <chrono>
#include <list>
#include "transactions.hpp"

class Block {
    public:
        Block(std::string prevHash, std::list<Transaction> body) {
            this->prevHash = prevHash;
            this->body = body;
            this->merkleRootHash = MerkleHash(body);
        }

        std::string Timestamp() {
            std::stringstream ss;
            ss << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %X");
            return ss.str();
        }

        std::string HeaderHash() {
            std::ostringstream sstream;
            sstream << prevHash << timestamp << version << merkleRootHash << nonce << difficulty;
            return hash(sstream.str());
        }

        bool isNonceValid(int nonce) {
            this->nonce = nonce;
            this->timestamp = CurrentTime();

            std::string hash = HeaderHash();
            std::string zeros(1, '0');
            if (hash.substr(0, difficulty) == zeros)
                return true;
            else return false;
        }

    private:
        std::string prevHash;
        time_t timestamp;
        double version = 1;
        std::string merkleRootHash;
        int nonce;
        int difficulty = 1;
        std::list<Transaction> body;

        time_t CurrentTime() {
            auto now = std::chrono::system_clock::now();
            return std::chrono::system_clock::to_time_t(now);
        }

        std::string MerkleHash(std::list<Transaction> t) {
            std::string allTrans = "";
            for (std::list<Transaction>::const_iterator iterator = t.begin(), end = t.end(); iterator != end; ++iterator) {
                allTrans += (*iterator).ID;
            }
            return hash(allTrans);
        }
};