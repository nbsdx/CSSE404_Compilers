CPP_FILES := $(wildcard src/lex/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

CC=g++
CFLAGS=-I/usr/include/boost -MMD -std=c++11

all: main

main: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

obj/%.o: src/lex/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm obj/* main

-include $(OBJFILES:.o=.d)
