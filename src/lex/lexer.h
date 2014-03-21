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
    void read_int(int fd, char *c);
    void read_operator(int fd, char *c);
    void read_comdiv(int fd, char *c);
    void comm_line(int fd, char *c);
    void comm_block(int fd, char *c);
    void read_name(int fd);
    void read_equal_assign(int fd);

    inline bool is_alpha( char c ){ return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
    void read_twochar_operator(int fd, char next, Operator::Op one, Operator::Op two);
    void maybe_read_twochar(int fd, char next, Operator::Op just);
};

} // End Namespace lex

#endif
