
#include "SmartTree.h"
#include "SmartTreeVisitor.h"

#include <iostream>
#include <string>

using namespace std;

namespace ir
{

CodeGenerator::CodeGenerator( const string &filename, Context *context )
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

    this->context = context;
}

void CodeGenerator::reserve_register()
{
    if( available_registers.empty() )
    {
        cerr << "ERROR: No available registers." << endl;
        exit( 1 );
    }
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
    stringstream s;
    
    // Set up the function header.
    size_t size = function_locals.size() * 8;
    function_header << "sub rsp, " << size << endl;
    
    // If we're in a class, store "this" from rdi.
    if( !current_class.empty() )
        function_header << "mov [rbp-8], rdi" << endl;
    
    s << function_header.str();
    s << function_body.str();
    s << function_footer.str();
    s << endl;

    function_header.str( "" );
    function_body.str( "" );
    function_footer.str( "" );

    if( !current_function.empty() )
    {
        auto a = functions.find( current_function );

        if( a == functions.end() )
            functions[ current_function ] = s.str();
        else
            (*a).second = s.str();

        text_header << "\tglobal " << current_function << endl;

        current_function = "";



        function_locals.clear();
    }
    else
    {
        text_body << s.str() << endl;
    }
}

void CodeGenerator::finalize_class()
{
    for( auto f : functions )
    {
        text_body << f.second << endl;
    }

    current_class = "";
    functions.clear();
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

    // Write Main Class.
    p->getMainClass()->visit( this );
    
    // Write other classes.
    for( auto a : p->getClasses() )
        a->visit( this );

    finalize_program();
}

void CodeGenerator::process( MainClass *mc )
{
    // Just process each statement
    // The statements themselves will manage register usage.
    function_header << "main:" << endl;
    function_header << "\tpush rbp" << endl;
    function_header << "\tmov rbp, rsp" << endl;

    for( auto a : mc->getBody() )
        a->visit( this );
    
    // Return 0.
    function_body << endl;
    function_body << "\txor rax, rax" << endl;
    function_body << "\tcall exit" << endl;

    function_header << "\tsub rsp, " << function_locals.size() * 8 << endl;

    text_body << function_header.str() << endl;
    text_body << function_body.str() << endl;
    text_body << function_footer.str() << endl;

    function_header.str( "" );
    function_body.str( "" );
    function_footer.str( "" );

    function_locals.clear();
}

void CodeGenerator::process( Class *c )
{
    // Not much to do. Just write the functions.
    current_class = c->getName();
    
    // Simple soultion. Jumps to previous definition by deafult
    // Note that this will work even if they have args on the 
    // stack since the stack point isn't being modified. Its
    // only inelegence is the lack of the old call being inlined.

    // Get the list of functions that the parent class has.
    if( !c->getParentName().empty() )
    {
        // Build the function list.
        for( string fname : context->getNamespace( c->getParentName() )->getEntryNames() )
        {
            stringstream s;

            s << c->getName() << "_" << fname << ":" << endl;
            s << "\tjmp " << c->getParentName() << "_" << fname << endl;

            // Add them to this list. 
            functions[ c->getName() + "_" + fname ] = s.str();
        }
        
        // Build the Member list. Initially, this is equal to the
        // parent's member list.
        vector<pair<string,size_t>> *vec = new vector<pair<string,size_t>>();
        vector<pair<string,size_t>> *par = class_members[ c->getParentName() ];
        vec->insert( vec->begin(), par->begin(), par->end() );

        class_members[ c->getName() ] = vec;
    }
    else
    {
        // Initialize this entry.
        class_members[ c->getName() ] = new vector<pair<string,size_t>>();
    }
    
    // Process Class Member Variables.
    for( auto a : c->getMembers() )
    {
        a->visit( this );
    }

    // Write a "Constructor" function that's just the class name.
    text_header << "\tglobal " << current_class << endl;
    text_body   << current_class << ":" << endl;
    text_body   << "\tpush rbp" << endl;
    text_body   << "\tmov rbp, rsp" << endl;
    text_body   << "\tmov rdi, " << class_members[ current_class ]->size() * 8 << endl;
    text_body   << "\tcall malloc" << endl;
    text_body   << "\tleave" << endl << "\tret" << endl;
    text_body   << endl;

    // Write a "Deconstructor" function that's just "delete_classname".
    text_header << "\tglobal delete_" << current_class << endl;
    text_body   << "delete_" << current_class << ":" << endl;
    text_body   << "\tpush rbp" << endl;
    text_body   << "\tmov rbp, rsp" << endl;
    text_body   << "\tcall free" << endl;
    text_body   << "\tleave" << endl << "\tret" << endl;

    // testing...
//    cout << "Members for class " << current_class << ":" << endl;
//    for( auto a : (*class_members[ current_class ]) )
//    {
//        cout << a.first << " : " << a.second << endl;
//    }

    // Process Functions.
    for( auto a : c->getFunctions() )
    {
        a->visit( this );
    }

    finalize_class();
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

    current_function = current_class + "_" + f->getName();

    function_header << current_function << ":" << endl;
 
    // Function prologue
    function_header << "\tpush rbp" << endl;
    function_header << "\tmov rbp, rsp" << endl;

    last_local = function_locals.insert( function_locals.begin(), make_pair("this", 0) );

    // At this point, we need to subtract from ESP when 
    // we want to add a new local variable. We can do this
    // from within the process( IStatement ) function as needed.
    // It's a little bit sketch, and it doesn't look pretty,
    // but it works.

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
void CodeGenerator::process( Formal *f )
{
    bool in_class = !current_class.empty();
    bool in_func  = !current_function.empty();

    if( in_class && in_func )
    {
        // Method Param.
        cerr << "Error: Passing params to functions is unsupported at this time." << endl;
    }
    else if( in_class )
    {
        // Member Var.
        if( class_members[ current_class ]->size() == 0 )
        {
            class_members[ current_class ]->push_back( make_pair( f->getName(), 0 ) );
        }
        else
        {
            size_t current = class_members[ current_class ]->back().second;
            class_members[ current_class ]->push_back( make_pair( f->getName(), current + 8 ) );
        }
    }
    else
    {
        // Error.
    }
}

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

void CodeGenerator::process( AssignmentStatement *a )
{
    if( a->getNew() )
    {
        if( function_locals.empty() )
            last_local = function_locals.insert( function_locals.begin(), make_pair( a->getDest(), 0 ) );
        else
            // We have a new var... save it and record it's address.
            last_local = function_locals.insert( function_locals.begin(), make_pair( a->getDest(), last_local->second + 8 ) );
    }
    
    // Destination is always a memory address, even if it's a new local var.
    // Get that address first.
    reserve_register();

    string value_reg = outreg.top();

    // Get the result of the RHS, store into value_reg.
    a->getValue()->visit( this );
    
    reserve_register();
    
    // It's a local
    auto res = function_locals.find( a->getDest() );
    if( res != function_locals.end() )
    {
        function_body << "\tmov " << outreg.top() << ", rbp" << endl;
        function_body << "\tsub " << outreg.top() << ", " << (res->second + 8) << endl;
    }
    else
    {
        // Check the class scope.
        for( auto au : (*class_members[ current_class ]) )
        {
            if( au.first.compare( a->getDest() ) == 0 )
            {
                // Get "this"
                function_body << "\tmov " << outreg.top() << ", QWORD [rbp-8]" << endl;
                // Get offset to var.
                function_body << "\tadd " << outreg.top() << ", " << au.second << endl;
                break;
            }
        }
    }

    // At this point, the dest address is in $outreg.top()
    function_body << "\tmov [" << outreg.top() << "], " << value_reg << endl;

    release_register();
    release_register();
}

void CodeGenerator::process( IfStatement *s )
{
    // ID used to track branch count to generate unique names.
    static int branch_count = 0;
    int branch_id = 0;

    // Need incase of nested if's.
    branch_id = branch_count++;
    
    reserve_register();

    s->getCondition()->visit( this );

    // jmp to on_true if [outreg.top()] neq 0 
    //     ...
    //     jmp end
    // on_true:
    //     ...
    // end:
    //     ...
    //     
    
    function_body << "\tcmp " << outreg.top() << ", 0" << endl;
    function_body << "\tjne on_true_" << branch_id << endl;
    
    release_register();

    for( auto a : s->getOnFalse() )
    {
        a->visit( this );
    }
    
    function_body << "\tjmp end_branch_" << branch_id << endl;
    function_body << "on_true_" << branch_id << ":" << endl;

    for( auto a : s->getOnTrue() )
    {
        a->visit( this );
    }

    function_body << "end_branch_" << branch_id << ":" << endl;
}

void CodeGenerator::process( WhileStatement *w )
{
    static int loop_count = 0;
    int loop_id = 0;

    loop_id = loop_count++;

    reserve_register();

    w->getCondition()->visit( this );
    
    function_body << "\tloop" << loop_id << ":" << endl;
    function_body << "\tcmp " << outreg.top() << ", 0" << endl;
    function_body << "\tjeq exit_loop" << loop_id << endl;

    release_register();

    for( auto a : w->getBody() )
    {
        a->visit( this );
    }

    function_body << "\tjmp loop" << loop_id << endl;
    function_body << "exit_loop" << loop_id << ":" << endl;
}

void CodeGenerator::process( MathExpression *m )
{
    string dest = outreg.top();
    string right_result;

    m->getLeft()->visit( this );
    function_body << "\tmov " << dest << ", " << outreg.top() << endl;

    // Not necessecary:
    // release_register();
    // reserve_register();
    
    reserve_register();
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

void CodeGenerator::process( CallExpression *c )
{
    string dest = outreg.top();

    reserve_register();
    c->getCaller()->visit( this );
    function_body << "\tmov rdi, " << outreg.top() << endl;
//    release_register();
    
    // TODO: Setup arguments.
    
    // Call Function;
    function_body << "\tcall " << c->getClass() << "_" << c->getFunction() << endl;
    function_body << "\tmov " << dest << ", rax" << endl;
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
    string lit = f->getLiteral();
    stringstream stream( lit );
    int i;
    char *endp;

    stream >> i;
    //long value;

    //value = strtol( lit.c_str(), &endp, 10 );

    //if( ( endp != lit.c_str() ) && ( *endp != '\0' ) )
    if( stream.fail() == false )
    {
        cout << "I'm a fucking number" << endl;
        function_body << "\tmov " << outreg.top() << ", " << i << endl;
    }
    else if( lit.compare( "true" ) == 0 )
    {
        function_body << "\tmov " << outreg.top() << ", " << 1 << endl;
    }
    else if( lit.compare( "false" ) == 0 ||
             lit.compare( "null" ) == 0 )
    {
        function_body << "\tmov " << outreg.top() << ", " << 0 << endl;
    }
    else if( lit.compare( "this" ) == 0 )
    {
        // XXX: This is bad. It assumes that the "this" pointer is located at $rbp-8
        function_body << "\tmov " << outreg.top() << ", QWORD [rbp-8]" << endl;
    }
    else
    {
        cout << "FOUND LOCAL" << endl;
        // We have a local/member variable.
        
        // Check the local scope.
        auto ret = function_locals.find( lit );
        if( ret != function_locals.end() )
        {
            function_body << "\tmov " << outreg.top() << ", QWORD [rbp-" << (ret->second + 8) << "]" << endl;
        }
        else
        {

            // Check the class scope.
            for( auto a : (*class_members[ current_class ]) )
            {
                if( a.first.compare( lit ) == 0 )
                {
                    function_body << "\tmov " << outreg.top() << ", QWORD [rbp-8]" << endl;
                    function_body << "\tmov " << outreg.top() << ", QWORD [" << outreg.top() << "+" << a.second << "]" << endl;
                    break;
                }
            }
        }
    }
}


} // End Namespace <ir>
