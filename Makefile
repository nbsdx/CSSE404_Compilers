CPP_FILES := $(wildcard src/util/*.cpp) $(wildcard src/lex/*.cpp) $(wildcard src/parse/*.cpp)
#OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

OBJ_FILES := obj/token.o obj/lexer.o obj/AST.o obj/main.o

CC=g++
CFLAGS=-I/usr/include/boost -MMD -std=c++11 -Wall

all: main

main: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

obj/main.o: src/parse/main.cpp obj/token.o obj/lexer.o obj/AST.o
	$(CC) $(CFLAGS) -c -o $@ $<

obj/AST.o: src/parse/AST.cpp src/parse/AST.h obj/token.o
	$(CC) $(CFLAGS) -c -o $@ $<

obj/lexer.o: src/lex/lexer.cpp src/lex/lexer.h obj/token.o
	$(CC) $(CFLAGS) -c -o $@ $<

obj/token.o: src/util/token.cpp src/util/token.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm obj/* main

-include $(OBJFILES:.o=.d)
