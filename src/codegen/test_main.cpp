
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
  
    PrintStatement *print = new PrintStatement( new NewExpression( "Test2" ) );
    m->addStatement( print );
/*
    FinalExpression *five = new FinalExpression( "5" );
    PrintStatement *print = new PrintStatement( five );
    m->addStatement( print );

    MathExpression *math = new MathExpression();
    math->setLeft( new FinalExpression( "10" ) );
    MathExpression *inner = new MathExpression();
    inner->setLeft( new FinalExpression( "15" ) );
    inner->setRight( new FinalExpression( "8" ) );
    inner->setOperator( MathExpression::Sub );
    math->setRight( inner );
    math->setOperator( MathExpression::Mul );

    MathExpression *m2 = new MathExpression();
    m2->setLeft( math );
    m2->setRight( new FinalExpression( "10" ) );
    m2->setOperator( MathExpression::Div );

    m->addStatement( new PrintStatement( m2 ) );
*/
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
