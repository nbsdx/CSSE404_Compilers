CC=g++
CFLAGS="-I/usr/include/boost"

all: main

lexer: token.o lexer.cpp 
	$(CC) $(CFLAGS) token.o -c lexer.cpp

token: token.cpp
	$(CC) $(CFLAGS) -c token.cpp

main: main.cpp lexer.o token.o
	$(CC) main.cpp
clean:
	rm -rf *.o a.out
