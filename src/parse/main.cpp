
#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <unistd.h>
#include "tt.hpp"
#include "../lex/lexer.h"

using namespace std;

struct EndOfStack : BasicToken
{
    EndOfStack() : BasicToken( "$$" ) {}
    ~EndOfStack() {}
};

bool match( BasicToken *t1, BasicToken *t2 )
{
    // Edge case - should never happen.
    if( dynamic_cast<NonTerminal*>( t1 ) && dynamic_cast<NonTerminal*>( t2 ) )
        return *dynamic_cast<NonTerminal*>( t1 ) == *dynamic_cast<NonTerminal*>( t2 );
    
    if( dynamic_cast<ReservedWord*>( t1 ) && dynamic_cast<ReservedWord*>( t2 ) )
        return *dynamic_cast<ReservedWord*>( t1 ) == *dynamic_cast<ReservedWord*>( t2 );
    
    if( dynamic_cast<Delimiter*>( t1 ) && dynamic_cast<Delimiter*>( t2 ) )
        return *dynamic_cast<Delimiter*>( t1 ) == *dynamic_cast<Delimiter*>( t2 );
    
    if( dynamic_cast<Operator*>( t1 ) && dynamic_cast<Operator*>( t2 ) )
        return *dynamic_cast<Operator*>( t1 ) == *dynamic_cast<Operator*>( t2 );
    
    if( dynamic_cast<Number*>( t1 ) && dynamic_cast<Number*>( t2 ) )
        return true;

    if( dynamic_cast<Identifier*>( t1 ) && dynamic_cast<Identifier*>( t2 ) )
        return true;

    return false;
}

void print_token( BasicToken *token )
{
    
    ReservedWord *rw;
    Operator *op;
    Delimiter *d;
    Identifier *id;
    Number *num;

    if( rw = dynamic_cast<ReservedWord*>( token ) )
        cout << "ReservedWord, [" << rw->line() << "," << rw->pos() << "]";
    else if( op = dynamic_cast<Operator*>( token ) )
        cout << "Operator, [" << op->line() << "," << op->pos() << "]";
    else if( d = dynamic_cast<Delimiter*>( token ) )
        cout << "Delimiter, [" << d->line() << "," << d->pos() << "]";
    else if( id = dynamic_cast<Identifier*>( token ) )
        cout << "ID, [" << id->line() << "," << id->pos() << "]";
    else if( num = dynamic_cast<Number*>( token ) )
        cout << "Integer, [" << num->line() << "," << num->pos() << "]";
    else if( dynamic_cast<NonTerminal*>( token ) )
        std::cout << "NonTerminal, ";
    else if( dynamic_cast<EndOfFileToken*>( token ) )
        std::cout << "EndOfFile, ";
    else if( dynamic_cast<EndOfStack*>( token ) )
        std::cout << "EndOfStack, ";
    else if( dynamic_cast<Epsilon*>( token ) )
    {
        std::cout << "Epsilon\n";
        return;
    }
    else
        std::cout << "ERROR: " << typeid( *token ).name() << " ";
    
    std::cout << "[" << token->raw() << "]"  << std::endl;
}

// Make Messages more visible:
enum Color {
    None = 0,
    Black, Red, Green, Yellow, Blue, Magenta, Cyan, White
};

string set_color( Color fg = None, Color bg = None )
{
    char num_s[3];
    string s = "\033[";
    if( !fg && !bg )
        s += "0";

    if( fg )
    {
        s += to_string( 29 + fg );
        if( bg ) 
            s += ";";
    }

    if( bg )
        s += to_string( 39 + bg );

    return s + "m";
};

int main( int argc, char **argv )
{
    // Initialize our Transition Table.
    TransitionTable tt;
    vector<BasicToken*>::reverse_iterator rit;
    int fd;

    if( argc != 2 )
    {
        std::cout << "Error please supply file\n";
        exit( 1 );
    }

    if( ( fd = open( argv[1], O_RDONLY ) ) == -1 )
    {
        std::cout << "Error opening file\n";
        exit( 1 );
    }
    
    // Lexer Output.
    vector<BasicToken*> pgm = lex::Lexer().lex( fd );

    for( BasicToken *token : pgm )
    {
        print_token( token );
    }

    // Stack of Symbols.
    stack<BasicToken*>  symbols;
    symbols.push( new EndOfStack() );

    // Current Prod ID
    int curProdGroup = 0;

    // Temp storage
    vector<BasicToken*> temp = tt.getRHSById( curProdGroup );

    // Push the Start State.
    for( rit = temp.rbegin() ; rit != temp.rend() ; ++rit )
    {
//       cout << "Pushing: "; print_token( *rit );
        symbols.push( *rit );
    }
    
    curProdGroup = 1;

    // Loop
    while( symbols.size() > 0 )
    {
/*
        if( dynamic_cast<EndOfFileToken*>( pgm[0] ) )
        {
            cout << "End of File" << endl;
        }
*/
        if( dynamic_cast<Epsilon*>( symbols.top() ) )
        {
            symbols.pop();
        }
        else if( dynamic_cast<EndOfStack*>( symbols.top() ) )
        {
            cout << "Hit bottom of stack." << endl;
            symbols.pop();
        }
        else if( match( symbols.top(), pgm[0] ) )
        {
            symbols.pop();
            pgm.erase( pgm.begin() );
        }
        else
        {
            if( dynamic_cast<NonTerminal*>( symbols.top() ) )
            {
                NonTerminal *nt = dynamic_cast<NonTerminal*>( symbols.top() );
                symbols.pop();

                int prod_group_id = tt.prodGroupId( nt->raw() );

                int next_prod_index;
                
                // These two are special cases....
                if( dynamic_cast<Number*>( pgm[0] ) )
                    next_prod_index = tt.getNextProdIndex( prod_group_id, "Integer" );
                else if( dynamic_cast<Identifier*>( pgm[0] ) )
                    next_prod_index = tt.getNextProdIndex( prod_group_id, "ID" );
                else
                    next_prod_index = tt.getNextProdIndex( prod_group_id, pgm[0]->raw() );

                temp = tt.getRHSById( next_prod_index );
                for( rit = temp.rbegin(); rit != temp.rend(); ++rit )
                {
                    symbols.push( *rit );
                }
            }
            else
            {
                ReservedWord *rw;
                Operator *op;
                Delimiter *d;
                Identifier *id;
                Number *num;

                BasicToken *token = pgm[0];
                
                cerr << set_color( Red );
                cerr << "Syntax Error: "; 
                if( rw = dynamic_cast<ReservedWord*>( token ) )
                    cerr << "Line " << rw->line() << " column: " << rw->pos() << endl;
                else if( op = dynamic_cast<Operator*>( token ) )
                    cerr << "Line " << op->line() << " column: " << op->pos() << endl;
                else if( d = dynamic_cast<Delimiter*>( token ) )
                    cerr << "Line " << d->line() << " column: " << d->pos() << endl;
                else if( id = dynamic_cast<Identifier*>( token ) )
                    cerr << "Line " << id->line() << " column: " << id->pos() << endl;
                else if( num = dynamic_cast<Number*>( token ) )
                    cerr << "Line " << num->line() << " column: " << num->pos() << endl;
                
                cerr << set_color() << "Found:    " << set_color( Blue ) << pgm.front()->raw() << endl;
                cerr << set_color() << "Expected: " << set_color( Green ) << symbols.top()->raw() << endl;
                cerr << set_color();
                exit( 1 );
            }
        }
    }

    // Empty stack, and end of file.
    if( dynamic_cast<EndOfFileToken*>( pgm[0] ) )
    {
        cerr << set_color( Cyan ) << "No Errors" << set_color() << endl;
    }
    else
    {
        cerr << set_color( Red ) << "Error: Could not consume all input tokens" << set_color() << endl;
    }

    return 0;
}
