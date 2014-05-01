
#include "SmartTree.h"
#include "SmartTreeVisitor.h"

#include <iostream>
#include <string>

using namespace std;

namespace ir
{

CodeGenerator::CodeGenerator( const string &filename )
{
    file.open( filename, fstream::binary | fstream::out | fstream::trunc );
}

void CodeGenerator::process( Program *p )
{
    // Write any init code that we need.
    file << "section .data" << endl;
    file << "\tprint_format_string db \"%d\", 10, 0" << endl;
    file << endl;

    file << "section .text" << endl;
    file << "\tglobal main" << endl;
    file << "\textern printf" << endl;
    file << "\textern exit" << endl;
    file << endl;

    file << "main:" << endl;

    // Write Main Class.
    p->getMainClass()->visit( this );
//    for( auto a : p->getClasses() )
//        a->visit( this );
    
    // Return 0.
    file << endl;
    file << "\tmov rax, 0" << endl;
    file << "\tcall exit" << endl;
}

void CodeGenerator::process( MainClass *mc )
{
    // Just process each statement
    // The statements themselves will manage register usage.
    for( auto a : mc->getBody() )
        this->visitIStatement( a );
}

void CodeGenerator::process( Class *c ){}
void CodeGenerator::process( Function *f ){}

// Depends on if we're in a class or a function declaration
// Note that this is ONLY used for representing class vars
// and function parameters. 
void CodeGenerator::process( Formal *f ){}

void CodeGenerator::process( PrintStatement *p )
{
    this->visitIExpression( p->getValue() );
    file << "\tmov rdi, print_format_string" << endl;
    file << "\tmov rsi, rdx" << endl;
    file << "\tmov rax, 0" << endl;
    file << "\tcall printf" << endl;
}

/*
 *  Things like 5, true, false, null, etc
 */
void CodeGenerator::process( FinalExpression *f )
{
    int i = stoi( f->getLiteral() );
    
    file << "\tmov rdx, " << i << endl;
}


} // End Namespace <ir>
