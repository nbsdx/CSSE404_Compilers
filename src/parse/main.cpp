
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
    if( dynamic_cast<ReservedWord*>( token ) )
        std::cout << "ReservedWord, ";
    else if( dynamic_cast<Operator*>( token ) )
        std::cout << "Operator, ";
    else if( dynamic_cast<Delimiter*>( token ) )
        std::cout << "Delimiter, ";
    else if( dynamic_cast<Identifier*>( token ) )
        std::cout << "ID, ";
    else if( dynamic_cast<Number*>( token ) )
        std::cout << "Integer, ";
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
        cout << "Pushing: "; print_token( *rit );
        symbols.push( *rit );
    }
    
    curProdGroup = 1;

    // Loop
    while( symbols.size() > 0 )
    {
        cout << "Testing: "; print_token( symbols.top() );
        cout << "Against: "; print_token( pgm[0] );

        if( dynamic_cast<Epsilon*>( symbols.top() ) )
        {
            symbols.pop();
        }
        else if( dynamic_cast<EndOfFileToken*>( symbols.top() ) )
        {
            cout << "Finished!" << endl;
            pgm.erase( pgm.begin() );
            symbols.pop();
        }
        else if( dynamic_cast<EndOfStack*>( symbols.top() ) )
        {
            cout << "Hit bottom of stack." << endl;
            symbols.pop();
        }
        else if( match( symbols.top(), pgm[0] ) )
        {
            cout << "MATCH!" << endl;
            symbols.pop();
            pgm.erase( pgm.begin() );
        }
        else
        {
            if( dynamic_cast<NonTerminal*>( symbols.top() ) )
            {
                NonTerminal *nt = dynamic_cast<NonTerminal*>( symbols.top() );
                cout << "Split NT: "; print_token( nt );
                symbols.pop();

                int prod_group_id = tt.prodGroupId( nt->raw() );
                cout << "ProdGroupID:  " << prod_group_id << endl;

                int next_prod_index;
                
                // These two are special cases....
                if( dynamic_cast<Number*>( pgm[0] ) )
                    next_prod_index = tt.getNextProdIndex( prod_group_id, "Integer" );
                else if( dynamic_cast<Identifier*>( pgm[0] ) )
                    next_prod_index = tt.getNextProdIndex( prod_group_id, "ID" );
                else
                    next_prod_index = tt.getNextProdIndex( prod_group_id, pgm[0]->raw() );

                cout << "NextProdINDX: " << next_prod_index << endl;
               
                cout << "Next Token:   "; print_token( pgm[0] );

                temp = tt.getRHSById( next_prod_index );
                for( rit = temp.rbegin(); rit != temp.rend(); ++rit )
                {
                    cout << "Pushing: "; print_token( *rit );
                    symbols.push( *rit );
                }
            }
            else
            {
                cout << "Error parsing Token: " << pgm.front()->raw() << endl;
                cout << "\tExpected: " << symbols.top()->raw() << endl;
                exit( 1 );
            }
        }
    }

    return 0;
}
