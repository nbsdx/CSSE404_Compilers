
#include "SmartTreeVisitor.h"

#include <iostream>

using namespace std;

namespace ir
{

void Visitor::visitIStatement( IStatement *s )
{
    if( dynamic_cast<PrintStatement*>( s ) )
        dynamic_cast<PrintStatement*>( s )->visit( this );
    else
        cerr << "Unsupported Statement type..." << endl;
}

void Visitor::visitIExpression( IExpression *e )
{
    if( dynamic_cast<FinalExpression*>( e ) )
        dynamic_cast<FinalExpression*>( e )->visit( this );
    else if( dynamic_cast<MathExpression*>( e ) )
        dynamic_cast<MathExpression*>( e )->visit( this );
    else
        cerr << "Unsupported Expression type..." << endl;
}

void PrintVisitor::process( Program *p )
{
    p->getMainClass()->visit( this );
    for( auto a : p->getClasses() )
        a->visit( this );
}

void PrintVisitor::process( MainClass *mc )
{
    cout << "class " << mc->getName() << endl << "{" << endl;
    cout << "\tpublic static void main( String[] args )" << endl 
         << "\t{" << endl;
    for( auto a : mc->getBody() )
    {
        cout << "\t\t";
        a->visit( this );
        cout << endl;
    }
    cout << "\t}" << endl << "}" << endl;
}

void PrintVisitor::process( Class *c )
{
    cout << "class " << c->getName();
    if( !c->getParentName().empty() )
        cout << " extends " << c->getParentName();
    cout << endl << "{" << endl;
    for( auto a : c->getMembers() )
    {
        cout << "\t";
        a->visit( this );
        cout << ";" << endl;
    }

    cout << endl;
    for( auto a : c->getFunctions() )
    {
        cout << "\t";
        a->visit( this );
        cout << endl;
    }
    cout << "}" << endl;
}

void PrintVisitor::process( Function *f )
{
    bool first = true;
    cout << "public " << f->getRetType() << " " << f->getName() << "( ";
    for( auto a : f->getArgs() )
    {
        if( !first )
            cout << ", ";

        a->visit( this );
        first = false; 
    }
    cout << " )" << endl << "\t" << "{" << endl;
    for( auto a : f->getBody() )
    {
        cout << "\t\t";
        a->visit( this );
        cout << endl;
    }
    cout << endl << "\t\t" << "return ";
    f->getRet()->visit( this );
    cout << ";" << endl << "\t" << "}";
}

void PrintVisitor::process( PrintStatement *p )
{
    cout << "System.out.println( ";
    p->getValue()->visit( this );
    cout << " );";
}

void PrintVisitor::process( AssignmentStatement *a )
{
    if( a->getNew() )
    {
        cout << "\t\t" << a->getType() << " ";
    }

    cout << a->getDest() << " = ";

    a->getValue()->visit( this );
}

void PrintVisitor::process( FinalExpression *f )
{
    cout << f->getLiteral();
}

void PrintVisitor::process( NewExpression *n )
{
    cout << "new " << n->getClass() << "()";
}

void PrintVisitor::process( CallExpression *c )
{
    bool first = true;
    c->getCaller()->visit( this );
    cout << "." << c->getFunction() << "( ";
    for( auto a : c->getArgs() )
    {
        if( !first )
            cout << ", ";

        a->visit( this );
        first = false; 
    }
    cout << " )";
}

void PrintVisitor::process( MathExpression *m )
{
    cout << "( ";
    m->getLeft()->visit( this );
    switch( m->getOperator() )
    {
    case MathExpression::Add: cout << " + "; break;
    case MathExpression::Sub: cout << " - "; break;
    case MathExpression::Mul: cout << " * "; break;
    case MathExpression::Div: cout << " / "; break;
    }
    m->getRight()->visit( this );
    cout << " )";
}

void PrintVisitor::process( Formal *f )
{
    cout << f->getType() << " " << f->getName();
}

} // End Namespace <ir>
