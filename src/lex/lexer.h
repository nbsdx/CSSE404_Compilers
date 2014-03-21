#ifndef LEXER_H
#define LEXER_H 1

#include "token.h"
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <vector>

namespace lex {

class Lexer
{
public:
    vector<Token> lex( int fd );
protected:
private:
    char c;
    vector<Token> pgm;
    bool is_one(char candidate, const char* group);
    int read_int(int fd, char *c);
    int read_operator(int fd, char *c);
    int read_comdiv(int fd, char *c);
    int comm_line(int fd, char *c);
    int comm_block(int fd, char *c);
};

} // End Namespace lex

#endif
