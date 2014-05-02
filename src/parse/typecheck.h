
#ifndef TYPECHECK_H
#define TYPECHECK_H 1

#include <functional>
#include <iostream>
#include <string>
#include "context.h"
#include "AST.h"
#include "../codegen/SmartTree.h"

using namespace std;

class TypeCheck
{
    Context *global;
    Context *second;
    string  cur_namespace;
    string  cur_function;

public:
    bool clean;
    int errs;
    TypeCheck();
    RTree *check( RTree *raw );

    RTree *postOrder( RTree *tree, 
                      function<RTree* (RTree*)> visit,
                      function<RTree* (RTree*)> leave );

    RTree *visit( RTree *node );
    RTree *leave( RTree *node );

    RTree *visit2( RTree *node );
    RTree *leave2( RTree *node );

    ir::Program *getIR ();
protected:
    ir::Program *pgm;
    void typeError (string excuse, RTree *node);
    void dotExprResolve (RTree *t);
    void resolveDexPrime (string ltype, RTree *t);
    string typeMatch (string a, string b);
    string matchAll (vector<RTree*> branches);
};

#endif
