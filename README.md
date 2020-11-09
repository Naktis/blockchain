
# Blockchain

Simplified blockchain which uses self made [hash function](https://github.com/Naktis/hash-generator) and does mining magic, written in C++.

Features:
- User and transaction generation
- Block mining
- Transaction execution

## Installation and execution:


1. Download the latest release

2. Download and install a C++ compiler (i.e. [GCC](https://gcc.gnu.org/))

3. Navigate to the directory of the program

4. Compile and link program files, i.e.

- If you use *GCC* with *GNU Make*, type `make main`

- If you use *GCC* without *GNU Make*, type:

`g++ -c blockchain.cpp hash.cpp iodata.cpp main.cpp`

`g++ -o main blockchain.o hash.o iodata.o main.o`

5. Run the program with: `./main` (unix) or `main` (windows)

6. Read the given function list and choose one by typing its number
