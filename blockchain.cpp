#include "blockchain.hpp"

void executeTransactions(std::vector<Transaction> trans) {
    std::vector<User> users = getUsers();

    for (int transIndex = 0; transIndex < trans.size(); transIndex ++) {
        int senderIndex = getUserIndexByKey(users, trans[transIndex].senderKey);
        int receiverIndex = getUserIndexByKey(users, trans[transIndex].receiverKey);

        users[receiverIndex].balance += trans[transIndex].amount;
        users[senderIndex].balance -= trans[transIndex].amount;
    }

    std::ofstream out ("users.txt");
    for (int i = 0; i < users.size(); i ++) {
        out << users[i].name << " " << users[i].publicKey << " " << users[i].balance << "\n";
    }
    out.close();
}

Block newBlock(std::vector<Transaction> trans) {
    int nonce = 0;
    Block b("", trans);
    while (!b.isNonceValid(nonce))
        nonce++;
    executeTransactions(trans);
    return b;
}

Block newBlock(Block previous, std::vector<Transaction> trans) {
    int nonce = 0;
    Block b(previous.HeaderHash(), trans);
    while (!b.isNonceValid(nonce))
        nonce++;
    executeTransactions(trans);
    return b;
}

void createWholeChain() {
    int blockCount, transCount;
    std::cout << "Block count: ";
    std::cin >> blockCount;
    std::cout << "Transactions in a block count: ";
    std::cin >> transCount;

    std::vector<Block> chain;
    std::ofstream out ("block_hashes.txt");

    // genesis block
    std::vector<Transaction> body = getNTransactions(transCount);
    chain.push_back(newBlock(body));
    out << chain[0].HeaderHash() 
        << " Time: " << chain[0].Timestamp()
        << " Nonce: " << chain[0].Nonce()<< "\n";

    // other blocks
    for (int i = 1; i < blockCount; i ++) {
        body = getNTransactions(transCount);
        chain.push_back(newBlock(chain[i-1], body));
        out << chain[i].HeaderHash() 
            << " Time: " << chain[i].Timestamp()
            << " Nonce: " << chain[i].Nonce() << "\n";
    }
    out.close();

    std::cout << "\nBlocks' hashes have been saved to block_hashes.txt\n\n";
}
