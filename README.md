
# Blockchain

Simplified blockchain which uses self made [hash function](https://github.com/Naktis/hash-generator) and mines blocks of transactions, written in C++

Features:
- User and transaction generation
- Block mining
- Transaction execution

## Installation and execution:


1. Download the latest release

2. Download and install a C++ compiler (i.e. [GCC](https://gcc.gnu.org/)) and (optionally) Make tool (i.e. [GNU Make](https://www.gnu.org/software/make/)).

3. Download and install [libbitcoin toolkit](https://github.com/libbitcoin/libbitcoin-system).

4. Adjust the PKG_CONFIG_PATH environment variable, i.e.

   `export PKG_CONFIG_PATH=/me/libbitcoin/lib/pkgconfig`(unix)

3. Navigate to the directory of the program

4. Compile and link program files, i.e.

- If you use *GCC* with *GNU Make*, type `make`

- If you use *GCC* without *GNU Make*, type:

  `g++ -std=c++11 -o main merkle.cpp hash.cpp iodata.cpp blockchain.cpp main.cpp $(pkg-config --cflags --libs libbitcoin)`

5. Run the program with: `./main` (unix) or `main` (windows)

6. Read the given function list and choose one by typing its number


## Versions

### v0.3
- Added the libbitcoin toolkit
- Replaced the recursive merkle root hash function with create_merkle() function, which is based on libbitcoin library.

### v0.2
- Created Merkle Root hash
- Added balance and transaction hash verification
- Made blocks of transactions compete with each other

### v0.1
- Created the first draft of the simplified blockchain
- Added user and transaction generation functions
- Implemented block mining and transaction execution
- Used both in-memory and in-file data storage
