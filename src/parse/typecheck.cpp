#include <utility>
#include <sstream>

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

string TypeCheck::matchAll (vector<RTree*> branches) 
{
    string ret = "";
    bool match = true;

    for( RTree *b: branches )
    {
        string type = b->getType();

        bool branch_nil = type.compare( "_nil" ) == 0;
        bool matches = type.compare( ret ) == 0;
        
        if( ret.empty() && !branch_nil ) 
        {
            ret = type;
        } 
        else if( !branch_nil && !matches ) 
        {
            match = false;
            typeError("Found " + type + " when expecting " + ret);
        }
    }
    if( !match ) 
        ret = "";

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

        if (pform.compare("while") == 0) 
        {
            tups = {{2, "_nil"}, {4, "_void"}};
            bool match = expectsThese(tups, branches);
            if (!match) 
                typeError("while statement non-void.");
        } 
        else if (pform.compare("if") == 0) 
        {
            tups = {{2, "_nil"}, {4, "_void"}, {6, "_void"}};
            bool match = expectsThese(tups, branches);
            if (!match) 
                typeError("If statement branch non-void.");
        } 
        else if (pform.compare("{") == 0) 
        {
            tups = {{1, "_void"}};
            bool match = expectsThese(tups, branches);
            if (!match) 
                typeError("Braced block returning non-void.");
        } 
        else if (pform.compare("System.out.println") == 0) 
        {
            tups = {{2, "int"}};
            bool match = expectsThese(tups, branches);
            if (!match) 
                typeError("System.out.println only accepts numbers.");
        } 
        else 
        {
            // Assignments
            // Cases: ID = Expr;        => Everything should be of type "ID"
            //        ID ID = Expr;     => Everything should be of type "ID"
            //        int ID = Expr;    => Everything should be of type "int"
            //        boolean ID = Expr;=> Everything should be of type "boolean"
            
            // I think this is correct.        
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
        if (!match.empty()) 
        {
            node->setType(match);
        } 
        else 
        {
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
    } 
    else if (tval.compare ("DotExpr_") == 0) 
    {

        // Structure:
        // . ID ( ...  ) DotExpr_
        // Needs to handle the argument types.

    }
    else if( tval.compare( "Literal" ) == 0 )
    {
        string match = matchAll( branches );
        if( match.empty() )
        {
            typeError( "Mismatched types in Literal." );
            node->setType( "_nil" );
        }
        else
            node->setType( match );
    }
    else if ( dynamic_cast<Identifier*>( rep )) 
    {
        // TODO: actually look this up
        //node->setType("_lookup");
        string type = global->typeof( tval );

        if( type.find( "class" ) == 0 )
        {
            // We have something like the LHS of:
            // MyClass mc = new MyClass();
            node->setType( tval ); // Make it look like the type.
        }
        else if( type.compare( "Undefined" ) == 0 )
        {
            // We have an undefined variable. Might be "mc" of the above example.
            // We should just set it to _nil, and let the expression handle it.
            node->setType( "_nil" );
        }
        else
        {
            // Otherwise we should just set it to the lookedup type.
            node->setType( type );
        }
    } 
    else if ( dynamic_cast<Number*>( rep )) 
    {
        node->setType("int");
    } 
    else if ( dynamic_cast<Delimiter*>( rep )) {
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
            case False:
            case Bool: node->setType("boolean"); break;
            case Null: node->setType("null"); break;
            case New: node->setType("_nil"); break;

            case Int: node->setType("int" ); break;
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
        // Check that the return type and argument types are defined.
        // Get the definition from the first pass
        string def = global->typeof( node->getBranches()[2]->printVal() );
        stringstream stream( def );
        string word;

        stream >> word; // Read "Function"

        while( stream >> word )
        {
            if( !( word.compare( "int" ) 
                || word.compare( "boolean" ) ) )
            {
                // Check if it's a userdefined type.
                string type = global->typeof( word );
                if( type.find( "class" ) != 0 )
                {
                    typeError( "Undefined class: " + type );
                }
            }
        }
        
        global->enter();
        
        // Add the formals in the param list to the namespace
        // [This happens automatically from the Formal case]
    }
    else if( tval.compare( "Formal" ) == 0 )
    {
        // We know this is a valid type, or at least we don't care 
        // if it's not.
        string name = node->getBranches()[1]->printVal();
        string type = node->getBranches()[0]->getBranches()[0]->printVal();

        global->add( name, type );

        cout << "Added [" << name << " : " << type << "]" << endl;
    }
    else if( tval.compare( "ClassVarDecl" ) == 0 )
    {
        string name = node->getBranches()[1]->printVal();
        string type = node->getBranches()[0]->getBranches()[0]->printVal();

        if( type.compare( "int" ) != 0 && type.compare( "boolean" ) != 0 )
        {
            if( global->typeof( type ).compare( "Undefined" ) == 0 )
            {
                typeError( "Error: Type " + type + " undefined." );
            }
        }
        global->add( name, type );

        cout << "Added new Class Var [" << name << " : " << type << "]" << endl;
    }
    else if( tval.compare( "DotExpr" ) == 0 )
    {
        auto branches = node->getBranches();
        // Dot expressions need to know about the shit to their left. 
        // If there is actually a dot, eval it here, and ignore it later
        // otherwise eval it later.
        if( branches.size() > 1 )
        {
            // Type of a stored variable. Should give back a namespace name.
            string type = global->typeof( branches[0]->printVal() );
            // We can only dot our own types.
            if( type.compare( "int" ) == 0 ||
                type.compare( "boolean" ) == 0 )
            {
                typeError( "Error: builtin type " + type + " has no functions" );
                branches[1]->setLeftType( "Error" );
            }
            else if( type.compare( "Undefined" ) == 0 )
            {
                typeError( "Error: Undefined variable " + branches[0]->printVal() );
                branches[1]->setLeftType( "Error" );
            }
            else
            {
                branches[1]->setLeftType( type );
            }
        }
    }
    else if( tval.compare( "DotExpr_" ) == 0 )
    {
        // We should have "left_type" set.
        // If not, error.

        if( node->getLeftType().empty() )
        {
            cerr << "Damnit Neil" << endl;
            exit( 2 );
        }

        if( node->getLeftType().compare( "Error" ) == 0 )
        {
            // The left side is errored, just set this to nil.
            node->setType( "_nil" );
        }
        else
        {
            // Not errored, has been set. We're good to go.
            // Need to check if the last branch is a DotExpr_,
            // and if it is, we need to set it's left_type
            // to the return value of this. Parameter type
            // checking will happen when leaving the node.

            auto branches = node->getBranches();

            // Get the function type
            string type = global->typeof( node->getLeftType(), branches[1]->printVal() );
            // Validate that it is a function
            if( type.find( "function" ) == 0 )
            {
                bool skip = true;
                stringstream stream( type );
                string temp;
                string ret_type;
                stream >> temp; // Consume "function"
                stream >> ret_type; // Get return type.

                // Can only be ) or another DotExpr_
                NonTerminal *token = dynamic_cast<NonTerminal*>( branches.back()->getVal() );

                if( token )
                    branches.back()->setLeftType( ret_type );
            }
            else
            {
                typeError( "Error: " + node->getLeftType() + "." + branches[1]->printVal() + " is not a function" );
                node->setType( "_nil" );
            }
        }
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

        // I dont think this should ever happen.
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
/*        RTree *b = branches[1];

        string type = branches[0]->getBranches()[0]->printVal();
        if( type.compare( "BuiltInType" ) == 0 )
            type = branches[0]->getBranches()[0]->getBranches()[0]->printVal();

        global->add( cur_namespace,
                     string( b->printVal() ),
                     type );

        cout << "Added new Class Var: " << b->printVal() << endl;
*/    }

    return node;
}
