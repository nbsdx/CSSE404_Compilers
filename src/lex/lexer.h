#ifndef LEXER_H
#define LEXER_H 1

#include "token.h"
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

namespace lex {

class Lexer
{
public:
    vector< shared_ptr<Token> > lex( int fd );
protected:
private:
    char c;
    vector< shared_ptr<Token> > pgm;
    bool is_one(char candidate, const char* group);
    int read_int(int fd, char *c);
    int read_operator(int fd, char *c);
    int read_comdiv(int fd, char *c);
    int comm_line(int fd, char *c);
    int comm_block(int fd, char *c);
    int read_name(int fd);

    inline bool is_alpha( char c ){ return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
};

} // End Namespace lex

#endif
