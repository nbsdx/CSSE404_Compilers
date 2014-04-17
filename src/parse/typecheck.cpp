
#include "typecheck.h"

using namespace std;

TypeCheck::TypeCheck()
{
    this->global = new Context( true );
}

RTree* TypeCheck::check( RTree *raw )
{
    global->enter();

    RTree *cleaned = postOrder( raw,
                                [this](RTree* t) { return this->visit( t ); },
                                [this](RTree* t) { return this->leave( t ); } );
    
    cout << "Global Namespace: " << endl;
    global->print();

    return cleaned;
}

RTree *TypeCheck::postOrder( RTree *tree, 
                             function<RTree* (RTree*)> visit,
                             function<RTree* (RTree*)> leave )
{
//    cout << "Enter TC::postOrder" << endl;

    if( !tree || tree->isLeaf() )
        return tree;

    vector< RTree* > branches = tree->getBranches();

    visit( tree );

    RTree *handle = new RTree( tree-> getVal() );

    for( RTree *branch : branches )
    {
        handle->insertSubT( postOrder( branch, visit, leave ) );
    }

    return leave( handle );
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
