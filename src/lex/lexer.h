#ifndef LEXER_H
#define LEXER_H 1

#include "token.h"
#include "lexer.hpp"
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
    inline bool is_plus( char c ) { return c == '+'; }
    inline bool is_minus( char c ) { return c == '-'; }
    inline bool is_number( char c ) { return c >= '0' && c <= '9'; }
};

} // End Namespace lex

#endif
