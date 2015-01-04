all: dashkov.o dbword.o client.o
	g++ -std=c++11 dashkov.o dbword.o client.o -g -o dashkovgen -lsqlite3

client.o: dashkov.o client.cpp
	g++ client.cpp -g -c -o client.o

dashkov.o: dashkov.cpp
	g++ dashkov.cpp -g -c -o dashkov.o

dbword.o: dashkov.o dbword.cpp
	g++ -std=c++11 dbword.cpp -g -c -o dbword.o

clean:
	rm -rf dashkovgen dashkovgen.dSYM dashkov.o client.o dbword.o
	rm -r build

