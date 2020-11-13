#include "blockchain.hpp"

void executeTransactions(std::vector<Transaction> trans, std::vector<User> &users) {
    for (int transIndex = 0; transIndex < trans.size(); transIndex ++) {
        int senderIndex = getUserIndexByKey(users, trans[transIndex].senderKey);
        int receiverIndex = getUserIndexByKey(users, trans[transIndex].receiverKey);

        users[receiverIndex].balance += trans[transIndex].amount;
        users[senderIndex].balance -= trans[transIndex].amount;
    }
    removeTransactions(trans);
    printUsers(users);
}

int searchForNonce(Block &b, int maxNonce) {
    int nonce = 0;
    while (!b.isNonceValid(nonce) && nonce != maxNonce)
        nonce++;
    return nonce;
}

Block mineBlock(int blockCount, int transCount, std::string previousHeader, std::vector<User> &users) {
    // generate mining candidates with random transactions
    std::vector<Block> candidatesOriginal;
    for (int i = 0; i < blockCount; i ++) {
        Block temp(previousHeader, getNTransactions(transCount));
        candidatesOriginal.push_back(temp);
    }

    std::mt19937 mt(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> randBlock(0, blockCount-1);
    bool wasNonceFound = false;
    int maxNonce = 0;
    int candIndex, winnerIndex = -1;
    std::vector<Block> candidates;
    do {
        maxNonce += 5000;
        // recover the list of candidates
        candidates = candidatesOriginal;

        // try searching for nonces with each candidate
        while (candidates.size() != 0) {
            // get random candidate index
            candIndex = randBlock(mt);
            if (candIndex > (candidates.size() - 1)) {
                candIndex = (candidates.size() - 1) * candIndex / (blockCount - 1);
            }

            // try to find a correct nonce
            if (searchForNonce(candidates[candIndex], maxNonce) != maxNonce) {
                winnerIndex = candIndex;
                break;
            } else {
                candidates.erase(candidates.begin()+candIndex);
            }
        }
        // if mining all candidates failed, try again with increased nonce limit
    } while (winnerIndex == -1);

    executeTransactions(candidates[winnerIndex].Body(), users);
    
    return candidates[winnerIndex];
}

void createWholeChain() {
    int blockCount, transCount, maxNonce, chainLength;
    std::cout << "Blocks to add to the chain: ";
    std::cin >> chainLength;
    std::cout << "Blocks to compete against each other: ";
    std::cin >> blockCount;
    std::cout << "Transactions in a block: ";
    std::cin >> transCount;

    std::vector<Block> chain;
    std::vector<User> users = getUsers();

    std::ofstream out ("block_hashes.txt");

    // genesis block
    chain.push_back(mineBlock(blockCount, transCount, "", users));
    out << chain[0].HeaderHash() 
        << " Time: " << chain[0].Timestamp()
        << " Nonce: " << chain[0].Nonce() << "\n";

    // other blocks
    for (int i = 1; i < chainLength; i ++) {
        chain.push_back(mineBlock(blockCount, transCount, chain[i-1].HeaderHash(), users));
        out << chain[i].HeaderHash() 
            << " Time: " << chain[i].Timestamp()
            << " Nonce: " << chain[i].Nonce() << "\n";
    }
    out.close();
    std::cout << "\nBlocks' hashes have been saved to block_hashes.txt\n\n";
}