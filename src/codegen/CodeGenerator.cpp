
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

    // Set up our register pool.
//    register_state[ "rax" ] = Clean;  // For simplicity, reserve RAX for ret value.
    register_state[ "rbx" ] = Clean;
    register_state[ "rcx" ] = Clean;
    register_state[ "rdx" ] = Clean;
//    register_state[ "rdi" ] = Clean;  // For simplicity, don't use these as GP 
//    register_state[ "rsi" ] = Clean;  // registers. Then they are "always" safe
    register_state[ "r8" ] = Clean;
    register_state[ "r9" ] = Clean;
    register_state[ "r10" ] = Clean;
    register_state[ "r11" ] = Clean;
    register_state[ "r12" ] = Clean;
    register_state[ "r13" ] = Clean;
    register_state[ "r14" ] = Clean;
    register_state[ "r15" ] = Clean;

    // Set all of our registers to clean. 
    for( auto i : register_state )
    {
        available_registers.push( i.first );
    }
}

void CodeGenerator::finalize_function()
{
    text_body << function_header.str();
    text_body << function_body.str();
    text_body << function_footer.str();

    function_header.str( "" );
    function_body.str( "" );
    function_footer.str( "" );
}

void CodeGenerator::finalize_program()
{
    file << section_data.str();
    file << endl; 
    file << text_header.str();
    file << text_body.str();

    file.close();
}

void CodeGenerator::process( Program *p )
{
    // Write any init code that we need.
    section_data << "section .data" << endl;
    section_data << "\tprint_format_string db \"%d\", 10, 0" << endl;

    text_header << "section .text" << endl;
    text_header << "\tglobal main" << endl;
    text_header << "\textern printf" << endl;
    text_header << "\textern exit" << endl;

    function_header << "main:" << endl;

    // Write Main Class.
    p->getMainClass()->visit( this );
    
    // Return 0.
    function_body << endl;
    function_body << "\txor rax, rax" << endl;
    function_body << "\tcall exit" << endl;

    finalize_function();

    // Write other classes.
//    for( auto a : p->getClasses() )
//        a->visit( this );

    finalize_program();
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
    outreg.push( available_registers.top() );
    register_state[ available_registers.top() ] = InUse;
    available_registers.pop();

    this->visitIExpression( p->getValue() );
    function_body << "\tmov rdi, print_format_string" << endl;
    function_body << "\tmov rsi, " << outreg.top() << endl;
    function_body << "\txor rax, rax" << endl;
    function_body << "\tcall printf" << endl;

    available_registers.push( outreg.top() );
    register_state[ outreg.top() ] = Clean;
    outreg.pop();
}

/*
 *  Things like 5, true, false, null, etc
 */
void CodeGenerator::process( FinalExpression *f )
{
    int i = stoi( f->getLiteral() );
    
    function_body << "\tmov " << outreg.top() << ", " << i << endl;
}


} // End Namespace <ir>
