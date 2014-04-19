#include <assert.h>
#include <vector>

#include "../util/token.h"
#include "AST.h"
#include "tt.hpp"

RTree::RTree(BasicToken* bt) 
{
    NonTerminal *nt = dynamic_cast<NonTerminal*>( bt );
    if (nt) this->leaf = false;
    else this->leaf = true;
    this->val = bt;
    this->cnt = 0;
    this->typed = false;
    this->type = "_untyped";
    this->left_type = "";
    this->error = false;
}

bool RTree::isLeaf () 
{
    return this->leaf;
}

void RTree::insertSubT (RTree *t) 
{
    assert (!leaf);
    if (t) {
        branches.push_back( t );
        cnt++;
    }
}

void RTree::printT () 
{
    // Dump tree lisp-style
    cout << "(" << this->val->raw() << " ";
    if (this->typed) {
        cout << ":" << this->type << ")";
    } else {
        cout << "\033[0;31m\]:UNTYPED" << "\[\033[0m\]";
    }


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
        cout << "  "; // lol
    }
    cout << "(";
    Util::print_token(this->val);

    cout << " ";
    if (this->typed) {
        cout << ":" << this->type;// << ")";
    } else {
        //cout << ")";
        cout << "\033[0;31m:UNTYPED" << "\033[0m";
    }

    if (this->error) {
        cout << "<===";
    }

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

int RTree::degree () {
    return this->cnt;
}

vector<RTree*> RTree::getBranches () {
    return this->branches;
}

BasicToken *RTree::getVal () {
    return this->val;
}

void RTree::setType (string str) {
    this->type = str;
    this->typed = true;
}

string RTree::getType () {
    return this->type;
}

bool RTree::getErr () {
    return this->error;
}

void RTree::setErr () {
    this->error = true;
}