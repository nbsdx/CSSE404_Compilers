#include <assert.h>

#include "AST.h"
#include "tt.hpp"

RTree::RTree(BasicToken* bt) 
{
    NonTerminal *nt = dynamic_cast<NonTerminal*>( bt );
    if (nt) this->leaf = false;
    else this->leaf = true;
    this->val = bt;
}

bool RTree::isLeaf () 
{
    return this->leaf;
}

void RTree::insertSubT (RTree *t) 
{
    assert (!leaf);
    if (t)
        branches.push_back( t );
}

void RTree::printT () 
{
    // Dump tree lisp-style
    cout << "(" << this->val->raw() << " ";
    for (RTree *t : branches) 
    {
        cout << "\n";
        t->printBranch(1);
    }
    cout << ")";
}

void RTree::printBranch (int depth) {
    for (int i = 0; i < depth; i++) 
    {
        cout << "    "; // lol
    }
    cout << "(" << this->val->raw() << " ";
    for (RTree *t : branches)
    {
        cout << "\n";
        t->printBranch (depth+1);
    }
    cout << ")";
}

string RTree::printVal () {
    return this->val->raw();
}
