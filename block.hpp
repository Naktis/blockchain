#pragma once

#include <chrono>
#include <list>
#include "transactions.hpp"

class Block {
    public:
        std::string GetTimestamp() {
            std::stringstream ss;
            ss << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %X");
            return ss.str();
        }

    private:
        std::string prevHash;
        time_t timestamp = CurrentTime();
        double version;
        std::string merkleRootHash;
        int nonce;
        int difficulty;
        std::list<Transaction> body;

        time_t CurrentTime() {
            auto now = std::chrono::system_clock::now();
            return std::chrono::system_clock::to_time_t(now);
        }
};