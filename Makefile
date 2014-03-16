CC=g++
CFLAGS="-I/usr/include/boost"

all: lexer

lexer: lexer.cpp
	$(CC) $(CFLAGS) -o lexer lexer.cpp
