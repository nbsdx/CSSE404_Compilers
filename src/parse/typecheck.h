
#ifndef TYPECHECK_H
#define TYPECHECK_H 1

#include <functional>
#include <iostream>
#include "context.h"
#include "AST.h"

using namespace std;

class TypeCheck
{
    Context *global;
    string  cur_namespace;

public:
    TypeCheck();
    RTree *check( RTree *raw );

    RTree *postOrder( RTree *tree, 
                      function<RTree* (RTree*)> visit,
                      function<RTree* (RTree*)> leave );

    RTree *visit( RTree *node );
    RTree *leave( RTree *node );

};

#endif
