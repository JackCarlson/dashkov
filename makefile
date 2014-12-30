all: dashkov.o dbword.o client.o
	g++ dashkov.o client.o -g -o dashkovgen

client.o: dashkov.o client.cpp
	g++ client.cpp -g -c -o client.o

dashkov.o: dashkov.cpp
	g++ dashkov.cpp -g -c -o dashkov.o

dbword.o: dashkov.o dbword.cpp
	g++ dbword.cpp -g -c -o dbword.o

clean:
	rm -rf dashkovgen dashkovgen.dSYM dashkov.o client.o dbword.o

