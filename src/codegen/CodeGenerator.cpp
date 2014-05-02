
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
    register_state[ "rcx" ] = Clean;  // Will probably be reserved for branching.
//    register_state[ "rdx" ] = Clean;  // Reserved for division.
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

void CodeGenerator::reserve_register()
{
    outreg.push( available_registers.top() );
    register_state[ available_registers.top() ] = InUse;
    available_registers.pop();
}

void CodeGenerator::release_register()
{
    available_registers.push( outreg.top() );
    register_state[ outreg.top() ] = Clean;
    outreg.pop();
}

void CodeGenerator::finalize_function()
{
    text_body << function_header.str();
    text_body << function_body.str();
    text_body << function_footer.str();
    text_body << endl;

    function_header.str( "" );
    function_body.str( "" );
    function_footer.str( "" );
}

void CodeGenerator::finalize_program()
{
    file << section_data.str();
    file << endl; 
    file << text_header.str();
    file << endl;
    file << text_body.str();

    file.close();
}

void CodeGenerator::process( Program *p )
{
    // Write any init code that we need.
    section_data << "section .data" << endl;
    section_data << "\tprint_format_string db \"%d\", 10, 0" << endl;

    text_header << "section .text" << endl;
    text_header << "\textern printf" << endl;
    text_header << "\textern malloc" << endl;
    text_header << "\textern free" << endl;
    text_header << "\textern exit" << endl;
    text_header << "\tglobal main" << endl;

    function_header << "main:" << endl;

    // Write Main Class.
    p->getMainClass()->visit( this );
    
    // Return 0.
    function_body << endl;
    function_body << "\txor rax, rax" << endl;
    function_body << "\tcall exit" << endl;

    finalize_function();

    // Write other classes.
    for( auto a : p->getClasses() )
        a->visit( this );

    finalize_program();
}

void CodeGenerator::process( MainClass *mc )
{
    // Just process each statement
    // The statements themselves will manage register usage.
    for( auto a : mc->getBody() )
        a->visit( this );
}

void CodeGenerator::process( Class *c )
{
    // Not much to do. Just write the functions.
    current_class = c->getName();
    
    // Write a "Constructor" function that's just the class name.
    // TODO: We need the Global Context so we have the size of the vars.
    text_header << "\tglobal " << current_class << endl;
    function_header << current_class << ":" << endl;
    function_header << "\tpush rbp" << endl;
    function_header << "\tmov rbp, rsp" << endl;
    // TODO: Replace 16 with the size of the struct gen'd from the Global Context.
    function_body << "\tmov rdi, " << 16 << endl;
    function_body << "\tcall malloc" << endl;
    // TODO: Error checking... [hahahahaha]
    function_footer << "\tleave" << endl << "\tret" << endl;
    finalize_function();

    // Write a "Deconstructor" function that's just "delete_classname".
    text_header << "\tglobal delete_" << current_class << endl;
    function_header << "delete_" << current_class << ":" << endl;
    function_header << "\tpush rbp" << endl;
    function_header << "\tmov rbp, rsp" << endl;
    function_body << "\tcall free" << endl;
    // TODO: Error checking... [hahahahaha]
    function_footer << "\tleave" << endl << "\tret" << endl;
    finalize_function();

    for( auto a : c->getFunctions() )
        a->visit( this );

    current_class = "";
}

void CodeGenerator::process( Function *f )
{
    /**
     *  Function Example:
     *      class Foo {
     *      public int foo( int arg1, int arg2 ) {
     *          // ...
     *      } }
     *
     *      Becomes:
     *      int Foo_foo( Foo *f, int arg1, int arg2 ) {...}
     */

    function_header << current_class << "_" << f->getName() << ":" << endl;
    text_header << "\tglobal " << current_class << "_" << f->getName() << endl;
 
    // Function prologue
    function_header << "\tpush rbp" << endl;
    function_header << "\tmov rbp, rsp" << endl;

    // At this point, we need to subtract from ESP when 
    // we want to add a new local variable. We can do this
    // from within the process( IStatement ) function as needed.


    // Write statements
    for( auto a : f->getBody() )
    {
        a->visit( this );
    }

    // Get the result from the return:
    reserve_register();
  
    f->getRet()->visit( this );
    
    function_footer << "\tmov rax, " << outreg.top() << endl;
    function_footer << "\tleave" << endl << "\tret" << endl;

    release_register();

    finalize_function();
}

// Depends on if we're in a class or a function declaration
// Note that this is ONLY used for representing class vars
// and function parameters. 
void CodeGenerator::process( Formal *f ){}

void CodeGenerator::process( PrintStatement *p )
{
    reserve_register();

    p->getValue()->visit( this );
    function_body << "\tmov rdi, print_format_string" << endl;
    function_body << "\tmov rsi, " << outreg.top() << endl;
    function_body << "\txor rax, rax" << endl;
    function_body << "\tcall printf" << endl;

    release_register();
}

void CodeGenerator::process( MathExpression *m )
{
    string dest = outreg.top();
    string right_result;

    reserve_register();
    m->getLeft()->visit( this );
    function_body << "\tmov " << dest << ", " << outreg.top() << endl;

    // Not necessecary:
    // release_register();
    // reserve_register();
    
    m->getRight()->visit( this );
    
    switch( m->getOperator() )
    {
    case MathExpression::Add:
        function_body << "\tadd " << dest << ", " << outreg.top() << endl;
        break;
    case MathExpression::Sub:
        function_body << "\tsub " << dest << ", " << outreg.top() << endl;
        break;
    case MathExpression::Mul:
        function_body << "\timul " << dest << ", " << outreg.top() << endl;
        break;
    case MathExpression::Div:
        right_result = outreg.top();
        
        // Store the contents of RDX
        reserve_register();
        function_body << "\tmov " << outreg.top() << ", " << "rdx" << endl;
        function_body << "\tmov rdx, 0" << endl;
        function_body << "\tmov rax, " << dest << endl;
        function_body << "\tidiv " << right_result << endl;
        function_body << "\tmov " << dest << ", rax" << endl;
        // restore RDX
        function_body << "\tmov rdx, " << outreg.top() << endl;
        release_register();
        break;
    }

    release_register();
}

void CodeGenerator::process( NewExpression *n )
{
    // Call the "classname" function.
    function_body << "\tcall " << n->getClass() << endl;
    function_body << "\tmov " << outreg.top() << ", rax" << endl;
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
