
#include "SmartTree.h"
#include "SmartTreeVisitor.h"

#include <iostream>

using namespace ir;

int main( int argc, char **argv )
{
    if( argc != 2 )
    {
        std::cerr << "Error: Please supply a filename to write to." << endl;
        exit( 1 );
    }

    Program *p = new Program();
    MainClass *m = new MainClass( "Test" );
    p->setMainClass( m );
    FinalExpression *five = new FinalExpression( "100" );
    PrintStatement *print = new PrintStatement( five );
    m->addStatement( print );

    Class *c = new Class( "Test2" );
    c->addMember( new Formal( "int", "number" ) );
    c->addMember( new Formal( "Test2", "hue" ) );

    Function *f = new Function( "func" );
    f->setRetType( "int" );
    f->setRet( new FinalExpression( "10" ) );
    f->addArg( new Formal( "int", "myarg" ) );
    f->addArg( new Formal( "int", "myarg2" ) );
    print = new PrintStatement( new FinalExpression( "25" ) );
    f->addStatement( print );

    c->addFunction( f );

    p->addClass( c );

    PrintVisitor *visitor = new PrintVisitor();
    CodeGenerator *gen = new CodeGenerator( argv[1] );

    p->visit( visitor );
    p->visit( gen );

}
