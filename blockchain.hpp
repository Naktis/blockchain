#pragma once

#include "block.hpp"

void executeTransactions(std::vector<Transaction> trans);
Block newBlock(std::vector<Transaction> trans);
Block newBlock(Block previous, std::vector<Transaction> trans);
void createWholeChain();