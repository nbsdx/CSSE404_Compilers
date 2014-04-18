#include <utility>

#include "typecheck.h"

using namespace std;

TypeCheck::TypeCheck()
{
    this->global = new Context( true );
//    this->second = new Context(false);
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
    global->print( 0 );

    cout << endl << endl;

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
        || tval.compare("MainClassDecl") == 0
        || tval.compare("MethodDecl") == 0)
    {
        node->setType( "_void" );
        global->leave();
    }
    else if( tval.compare("ClassDecls") == 0
            || tval.compare("ClassBody") == 0
            || tval.compare("Program") == 0
            || tval.compare("ClassHeader") == 0
            || tval.compare("ClassDeclRHS") == 0
            || tval.compare("ClassVarDecl") == 0 ) 
    {
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
    } else if (tval.compare ("DotExpr") == 0) {
        // one or two branches ONLY
        cout << "We have a DotExpr" << endl;
        if (branches.size() > 1) {
            string match = typeMatch(branches[0]->getType(), branches[1]->getType());
            if (!match.empty()) {
                node->setType(match);
            } else {
                typeError("Types did not match");
            }
        } else {
            // Only one branch
            node->setType(branches[0]->getType());
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

    if (tval.compare ("ClassDecl") == 0 )
    {
        string classname = node->getBranches()[0]->getBranches()[1]->printVal();
        cout << "\n\nENTERING CLASS: " << classname << endl;

        // Add the Class namespace to this one.
        global->enter();
        global->merge( global->getNamespace( classname ) );
        
        global->print( 0 );
    }
    else if( tval.compare("MainClassDecl") == 0 )
    {
        // Nothing else to do here; Enter for consistency
        // Main class is special and can't have functions/
        // or vatriables other than the main function.
        global->enter();

    } 
    else if( tval.compare("MethodDecl") == 0 )
    {
        // Add the formals in the param list to the namespace.

        global->enter();

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

        cur_namespace = string( b->printVal() );
        global->add( cur_namespace, classContext );
        global->add( cur_namespace, string( "class " ) + b->printVal() );

        cout << "Added new Namespace: " << b->printVal() << endl;
    }
    else if( tval.compare( "ClassDeclRHS" ) == 0 )
    {
        if( branches[0]->printVal().compare( "{" ) != 0 )
        {
            // We have an extention type.
            string type = branches[1]->printVal();
            
            // Make sure it's been defined.
            if( !global->defined( type ) )
            {
                typeError( "Cannot extend undefined class " + type );
            }
            else
            {
                global->setType( cur_namespace, global->typeof( cur_namespace ) + " " + type );
            }
        }
    }
    else if( tval.compare( "MethodDecl" ) == 0 )
    {
        RTree *b = branches[2];
        string value;

        // Classifier
        value += "function ";
        // Return Type
        string s = branches[1]->getBranches()[0]->printVal();
        if( s.compare( "BuiltInType" ) == 0 )
            s = branches[1]->getBranches()[0]->getBranches()[0]->printVal();
        value += s;

        // Add without checking return type. Check on second pass.
        global->add( cur_namespace,
                     string( b->printVal() ),
                     value );

        cur_function = b->printVal();

        cout << "Added new Method: " << b->printVal() << endl;
    }
    else if( tval.compare( "Formal" ) == 0 )
    {
        string type = global->typeof( cur_namespace,
                                      cur_function );

        if( type.compare( "Undefined" ) == 0 )
        {
            cout << "ERROR: " << cur_namespace << "::" << cur_function << " is undefined." << endl;
            exit( 1 );
        }
        
        // Two types: BuiltIn, User
        string s = branches[0]->getBranches()[0]->printVal();
        if( s.compare( "BuiltInType" ) == 0 )
            s = branches[0]->getBranches()[0]->getBranches()[0]->printVal();
        
        type += " ";
        type += s;

        global->setType( cur_namespace,
                         cur_function,
                         type );
    }
    else if( tval.compare( "ClassVarDecl" ) == 0 )
    {
        RTree *b = branches[1];

        string type = branches[0]->getBranches()[0]->printVal();
        if( type.compare( "BuiltInType" ) == 0 )
            type = branches[0]->getBranches()[0]->getBranches()[0]->printVal();

        global->add( cur_namespace,
                     string( b->printVal() ),
                     type );

        cout << "Added new Class Var: " << b->printVal() << endl;
    }

    return node;
}
