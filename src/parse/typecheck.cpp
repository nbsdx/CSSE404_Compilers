#include <utility>

#include "typecheck.h"

using namespace std;

TypeCheck::TypeCheck()
{
    this->global = new Context( true );
    this->second = new Context(false);
}

RTree* TypeCheck::check( RTree *raw )
{
    global->enter();

    RTree *cleaned = postOrder( raw,
                                [this](RTree* t) { return this->visit( t ); },
                                [this](RTree* t) { return this->leave( t ); } );

    cleaned = postOrder( cleaned,
                                [this](RTree* t) { return this->visit2( t ); },
                                [this](RTree* t) { return this->leave2( t ); } );

    cout << "Global Namespace: " << endl;
    global->print();

    return cleaned;
}

RTree *TypeCheck::postOrder( RTree *tree, 
                             function<RTree* (RTree*)> visit,
                             function<RTree* (RTree*)> leave )
{
    if (!tree->isLeaf()) {
        vector< RTree* > branches = tree->getBranches();

        visit( tree );

        RTree *handle = new RTree( tree-> getVal() );

        for( RTree *branch : branches )
        {
            handle->insertSubT( postOrder( branch, visit, leave ) );
        }
        return leave( handle );
    } else {
        tree = visit( tree );
        tree = leave( tree );
        return tree;
    }
}

void TypeCheck::typeError (string excuse) {
    cerr << excuse << endl;
    this->clean = false;
}

typedef pair<int, string> expect;

bool expectsThese(vector<expect> tups, vector<RTree*> branches) {
    bool ret = true;
    for (expect e : tups) {
        int fst = e.first;
        string exp = e.second;
        string act = branches[fst]->getType();

        if (exp.compare(act) != 0) ret = false;
    }
    return ret;
}

string TypeCheck::typeMatch (string a, string b) {
    string nil ("_nil");
    if (nil.compare(a) == 0) {
        return b;
    } else if (nil.compare(b) == 0) {
        return a;
    } else if (a.compare(b) == 0) {
        return a;
    } else {
        return "";
    }
}

string TypeCheck::matchAll (vector<RTree*> branches) {
    string ret = "";
    bool match = true;
    for (RTree *b: branches) {
        string type = b->getType();
        bool isNil = type.compare("_nil") == 0;
        bool matches = ret.compare(type) == 0;
        if (ret.empty() && !isNil) {
            ret = type;
        } else if (!isNil && !matches) {
            match = false;
            typeError("Found " + type + " when expecting " + ret);
        }
    }
    if (!match) ret = "";
    return ret;
}

RTree *TypeCheck::leave2( RTree *node ) {
    string tval = node->printVal();
    BasicToken* rep = node->getVal();
    vector<RTree*> branches = node->getBranches();
    int deg = branches.size();

    if (tval.compare ("ClassDecl") == 0
        || tval.compare("ClassDecls") == 0
        || tval.compare("MainClassDecl") == 0
        || tval.compare("ClassBody") == 0
        || tval.compare("Program") == 0
        || tval.compare("ClassHeader") == 0
        || tval.compare("ClassDeclRHS") == 0
        || tval.compare("ClassVarDecl") == 0
        || tval.compare("MethodDecl") == 0)
    {
        // TODO: These should probably check the correct subtrees
        // and ensure that they are void.
        // can do this with matchAll()
        second->leave();
        node->setType("_void");
    } else if (tval.compare ("Stmt") == 0) {
        // Stmts always return void
        node->setType("_void");
        // TODO: Stmt should also have multiple forms requiring checks
        //       and throw errors when unhappy.
        vector<expect> tups;
        string pform = branches[0]->printVal();
        if (pform.compare("while") == 0) {
            tups = {{2, "_nil"}, {4, "_void"}};
            bool match = expectsThese(tups, branches);
            if (!match) typeError("while statement non-void.");
        } else if (pform.compare("if") == 0) {
            tups = {{2, "_nil"}, {4, "_void"}, {6, "_void"}};
            bool match = expectsThese(tups, branches);
            if (!match) typeError("If statement branch non-void.");
        } else if (pform.compare("{") == 0) {
            tups = {{1, "_void"}};
            bool match = expectsThese(tups, branches);
            if (!match) typeError("Braced block returning non-void.");
        } else if (pform.compare("System.out.println") == 0) {
            tups = {{2, "int"}};
            bool match = expectsThese(tups, branches);
            if (!match) typeError("System.out.println only accepts numbers.");
        } else {
            // Assignments
            string match = matchAll(branches);
            if (match.empty())
                typeError("Mismatched types in assignment.");

        }
    } else if (tval.compare ("StmtLst") == 0) {
        node->setType("_void");
    } else if (tval.compare ("MultExpr") == 0
              || tval.compare("AddExpr") == 0
              || tval.compare("NegExpr") == 0
              || tval.compare("MultExpr_") == 0
              || tval.compare("AddExpr_") == 0
              || tval.compare("BoolExpr") == 0
              || tval.compare("BoolExpr_") == 0
              || tval.compare("Expr") == 0
              || tval.compare("DotExpr") == 0
              || tval.compare("StmtRHS") == 0) {

        //bool matchAll (vector<RTree*> branches) {
        string match = matchAll(branches);
        if (!match.empty()) {
            node->setType(match);
        } else {
            // TODO: Could insert an error type here and proceed.
        }
    } else if (tval.compare ("DotExpr_") == 0) {
            // Big mess. needs method lookups
            // TODO: Actually look things up
            //node->setType("_lookup");
    } else if ( dynamic_cast<Identifier*>( rep )) {
        // TODO: actually look this up
        //node->setType("_lookup");
    } else if ( dynamic_cast<Number*>( rep )) {
        node->setType("int");
    } else if ( dynamic_cast<Delimiter*>( rep )) {
        node->setType("_nil");
    } else if ( dynamic_cast<Operator*>( rep )) {
        Operator* op = dynamic_cast<Operator*>( rep );
        switch (op->token()) {
            // These operators can operate on any type
            //  (except for void)
            case EqualEq:  // Fall through
            case NEqual:   //      v
            case And:      //      v
            case Or:       //      v
            case Not: node->setType("_nil"); break;
            // The rest (+-/*<<=>=>) only work with numbers
            default: node->setType("int");
        };
    } else if ( dynamic_cast<ReservedWord*>( rep )) {
        ReservedWord* rw = dynamic_cast<ReservedWord*>( rep );
        switch (rw->token()) {
            case True: 
            case False: node->setType("boolean"); break;
            case Null: node->setType("null"); break;
            case New: node->setType("_nil"); break;
            // TODO: Check these
            default: node->setType("_void"); break;
        };
    }

    return node;
}

RTree *TypeCheck::visit2( RTree *node ) {
    string tval = node->printVal();
    BasicToken* rep = node->getVal();

    if (tval.compare ("ClassDecl")
        || tval.compare("MainClassDecl")
        || tval.compare("MethodDecl"))
    {
        second->enter();
    }

    return node;
}

RTree *TypeCheck::leave( RTree *node )
{
//    cout << "Enter TC::leave" << endl;

    string tval = node->printVal();
    vector< RTree* > branches = node->getBranches();

    string cmp( "Class Decl" );
    string typ( "Type" );

    if( tval.compare( "ClassDecl" ) == 0 )
    {
        cout << "Up one level" << endl;
//        global->leave();
    }
    else if( tval.compare( "Type" ) != 0 && branches.size() == 1 )
    {
        return branches[0];
    }

    return node;
}

RTree *TypeCheck::visit( RTree *node )
{
    string tval = node->printVal();
    vector< RTree* > branches = node->getBranches();

    if( tval.compare( "ClassDecl" ) == 0 )
    {
        RTree *b = branches[0]->getBranches()[1];
        
        Context *classContext = new Context( true );
        classContext->enter();

        global->add( string( b->printVal() ), classContext );
        cur_namespace = string( b->printVal() );

        cout << "Added new Namespace: " << b->printVal() << endl;
    }
    else if( tval.compare( "MethodDecl" ) == 0 )
    {
        RTree *b = branches[2];

        global->add( cur_namespace,
                     string( b->printVal() ),
                     "value-shit-fix-later" );

        cout << "Added new Method: " << b->printVal() << endl;
    }
    else if( tval.compare( "ClassVarDecl" ) == 0 )
    {
        RTree *b = branches[1];

        global->add( cur_namespace,
                     string( b->printVal() ),
                     "VAR_TYPE_ADD_NOW" );

        cout << "Added new Class Var: " << b->printVal() << endl;
    }

    return node;
}
