
#include "SmartTree.h"
#include "SmartTreeVisitor.h"
#include "../parse/context.h"

#include <iostream>

using namespace ir;

int main( int argc, char **argv )
{
    if( argc != 2 )
    {
        std::cerr << "Error: Please supply a filename to write to." << endl;
        exit( 1 );
    }

    Context *global = new Context();
    Context *test = new Context();
    test->enter();
    test->add( "foo", "function int" );
    Context *test2 = new Context();

    global->add( "Test", test );
    global->add( "Test2", test2 );

    Program *p = new Program();
    MainClass *m = new MainClass( "Main" );

    CallExpression *call = new CallExpression();
    call->setCaller( new NewExpression( "Test2" ) );
    call->setClass( "Test2" );
    call->setFunction( "foo" );

    m->addStatement( new PrintStatement( call ) );

    p->setMainClass( m );
  
    Class *c = new Class( "Test" );
    Function *f = new Function( "foo" );
    f->setRetType( "int" );
    f->setRet( new FinalExpression( "0" ) );
    c->addFunction( f );

    Class *c2 = new Class( "Test2" );
    c2->setParent( c );
    c2->setParentName( "Test" );

    p->addClass( c );
    p->addClass( c2 );

    PrintVisitor *visitor = new PrintVisitor();
    CodeGenerator *gen = new CodeGenerator( argv[1], global );

    p->visit( visitor );
    p->visit( gen );

}
