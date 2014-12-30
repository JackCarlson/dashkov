all: dashkov.cpp
	g++ client.cpp dashkov.cpp -g -o dashkovgen

clean:
	rm -rf dashkovgen
