#CPP_FILES := $(wildcard src/util/*.cpp) $(wildcard src/lex/*.cpp) $(wildcard src/parse/*.cpp)
#OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

OBJ_FILES := obj/token.o obj/lexer.o obj/AST.o obj/main.o obj/context.o obj/typecheck.o

CC=g++
CFLAGS=-MMD -std=c++11 -Wall -g

all: main

main: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

obj/main.o: src/parse/main.cpp src/parse/tt.hpp obj/token.o obj/lexer.o obj/AST.o obj/context.o obj/typecheck.o
	$(CC) $(CFLAGS) -c -o $@ $<

obj/typecheck.o: src/parse/typecheck.cpp src/parse/typecheck.h obj/AST.o
	$(CC) $(CFLAGS) -c -o $@ $<

obj/context.o: src/parse/context.cpp src/parse/context.h
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
