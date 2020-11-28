#pragma once

#include "iodata.hpp"
#include "merkle.hpp"

class Block {
    public:
        Block(std::string prevHash, std::vector<Transaction> body) {
            this->prevHash = prevHash;
            this->body = body;
            
            bc::hash_list idList = convertToHashList(body);
            this->merkleRootHash = create_merkle(idList);
        }

        std::string Timestamp() {
            std::stringstream ss;
            ss << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %X");
            return ss.str();
        }

        std::string HeaderHash() {
            std::ostringstream sstream;
            sstream << prevHash << timestamp << version 
                    << bc::encode_base16(merkleRootHash) << nonce << difficulty;
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
        bc::hash_digest merkleRootHash;
        int nonce;
        int difficulty = 3;
        std::vector<Transaction> body;

        time_t currentTime() {
            auto now = std::chrono::system_clock::now();
            return std::chrono::system_clock::to_time_t(now);
        }

        bc::hash_list convertToHashList(std::vector<Transaction> transVector) {
            bc::hash_list transIDList;
            char idAsCharList[65];

            if (transVector.empty()) {
                transIDList.push_back(bc::hash_literal(idAsCharList));
                return transIDList;
            }

            for (int i = 0; i < transVector.size(); i++) {
                // convert the transaction's ID string to the character list
                std::strcpy(idAsCharList, transVector[i].ID.c_str());
                // add the converted ID to the hash list
                transIDList.push_back(bc::hash_literal(idAsCharList));
            }

            return transIDList;
        }
};