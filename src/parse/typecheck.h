
#ifndef TYPECHECK_H
#define TYPECHECK_H 1

#include <functional>
#include <iostream>
#include <string>
#include "context.h"
#include "AST.h"

using namespace std;

class TypeCheck
{
    Context *global;
    Context *second;
    string  cur_namespace;

public:
    bool clean;
    TypeCheck();
    RTree *check( RTree *raw );

    RTree *postOrder( RTree *tree, 
                      function<RTree* (RTree*)> visit,
                      function<RTree* (RTree*)> leave );

    RTree *visit( RTree *node );
    RTree *leave( RTree *node );

    RTree *visit2( RTree *node );
    RTree *leave2( RTree *node );
protected:
    void typeError (string excuse);

    string typeMatch (string a, string b);
    string matchAll (vector<RTree*> branches);
};

#endif
