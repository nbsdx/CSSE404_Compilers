
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
    Context *test3 = new Context();
    test3->enter();
    test3->add( "foo", "function int" );

    global->add( "Test", test );
    global->add( "Test2", test2 );
    global->add( "Test3", test3 );

    Program *p = new Program();
    MainClass *m = new MainClass( "Main" );

    AssignmentStatement *local = new AssignmentStatement();
    local->setNew( true );
    local->setType( "int" );
    local->setDest( "ret" );
    local->setValue( new FinalExpression( "0" ) );

    m->addStatement( local );

    AssignmentStatement *aexp = new AssignmentStatement();
    aexp->setNew( true );
    aexp->setDest( "test_var" );
    aexp->setType( "Test3" );
    aexp->setValue( new NewExpression( "Test3" ) );

    m->addStatement( aexp );

    CallExpression *call = new CallExpression();
    call->setCaller( new FinalExpression( "test_var" ) );
    call->setClass( "Test3" );
    call->setFunction( "foo" );

    local = new AssignmentStatement();
    local->setNew( false );
    local->setDest( "ret" );
    local->setValue( call );

    m->addStatement( local );

    call = new CallExpression();
    call->setCaller( new FinalExpression( "test_var" ) );
    call->setClass( "Test3" );
    call->setFunction( "bar" );

    local = new AssignmentStatement();
    local->setNew( false );
    local->setDest( "ret" );
    local->setValue( call );

    m->addStatement( local );

    p->setMainClass( m );
  
    Class *c = new Class( "Test" );
    c->addMember( new Formal( "int", "member1" ) );
    c->addMember( new Formal( "bool", "member2" ) );
    Function *f = new Function( "foo" );
    f->setRetType( "int" );
    f->setRet( new FinalExpression( "0" ) );

    AssignmentStatement *as = new AssignmentStatement();
    as->setDest( "member1" );
    as->setType( "int" );
    as->setValue( new FinalExpression( "100" ) );

    f->addStatement( as );
    f->addStatement( new PrintStatement( new FinalExpression( "member1" ) ) );

    c->addFunction( f );

    Class *c2 = new Class( "Test2" );
    c2->setParent( c );
    c2->setParentName( "Test" );

    Class *c3 = new Class( "Test3" );
    c3->addMember( new Formal( "Test2", "some_local_instance" ) );
    c3->setParent( c );
    c3->setParentName( "Test" );
    f = new Function( "bar" );
    f->setRetType( "int" );
    f->setRet( new FinalExpression( "1" ) );
    f->addStatement( new PrintStatement( new FinalExpression( "member1" ) ) );
    c3->addFunction( f );

    p->addClass( c );
    p->addClass( c2 );
    p->addClass( c3 );

    PrintVisitor *visitor = new PrintVisitor();
    CodeGenerator *gen = new CodeGenerator( argv[1], global );

    p->visit( visitor );
    p->visit( gen );

}
