main: hash.o iodata.o blockchain.o
	g++ -c main.cpp
	g++ -o run main.o blockchain.o iodata.o hash.o
blockchain:
	g++ -c blockchain.cpp
iodata:
	g++ -c iodata.cpp
hash:
	g++ -c hash.cpp
clean:
	del *.o *.exe