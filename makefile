main:
	g++ -std=c++11 -o main merkle.cpp hash.cpp iodata.cpp blockchain.cpp main.cpp $$(pkg-config --cflags --libs libbitcoin)
clean-win:
	del *.o *.exe
clean-unix:
	rm *.o main