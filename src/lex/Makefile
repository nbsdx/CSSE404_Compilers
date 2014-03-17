CC = g++
INCPATH = -I/usr/include/boost
OPTS = -std=c++11 -fPIC

CXXFLAGS = $(INCPATH) $(OPTS)

all: main

token: token.h token.cpp
	$(CC) $(CXXFLAGS) -c token.cpp

lexer: token.o lexer.cpp lexer.hpp lexer.h
	$(CC) $(CXXFLAGS) token.o -c lexer.hpp lexer.cpp lexer.h

main: main.cpp token.o lexer.o 
	$(CC) $(CXXFLAGS) lexer.o main.cpp

clean:
	rm -rf *.o a.out
