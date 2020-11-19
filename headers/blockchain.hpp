#pragma once

#include "block.hpp"

void executeTransactions(std::vector<Transaction> trans, std::vector<User>& users);
int searchForNonce(Block& b, int maxNonce);
void printChain(std::vector<Block>& chain);
Block mineBlock(int blockCount, int transCount, std::string previousHeader, std::vector<User>& users);
void createWholeChain();