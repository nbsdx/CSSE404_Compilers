#ifndef PARSER_H
#define PARSER_H 1

#include <vector>
#include <utility>

#include "tt.hpp"
#include "../lex/lexer.h"
#include "AST.h"

class Parser {
    public:
    RTree *parse (vector<BasicToken*> pgm);
    protected:
    bool match( BasicToken *t1, BasicToken *t2 );
    pair<int,int> getPos(BasicToken *t);
};

#endif