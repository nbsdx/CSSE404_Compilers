
#include <iostream>
#include <utility>
#include <vector>
#include "tt.hpp"

using namespace std;

int main()
{
    // Initialize our Transition Table.
    TransitionTable tt;
    vector<BasicToken*>::const_iterator it;

    // Test some transitions
//    cout << tt.getNextProdIndex( 29, "Integer" ) << endl;
//    cout << tt.getNextProdIndex( 17, "int" ) << endl;
//    cout << tt.getNextProdIndex( 17, "boolean" ) << endl;
//    cout << tt.getNextProdIndex( 17, "ID" ) << endl << endl;

    // Get a production to test.
//    Production prod = tt.getProduction( 10 );
    

//    cout << "Prod: [" << prod.lhs << "]" << endl;
//    for( it = prod.rhs[0].begin(); it != prod.rhs[0].end(); ++it )

    for( int i = 0; i < 68; ++i )
    {
        vector<BasicToken*> prod = tt.getRHSById( i );
        
        for( BasicToken *token : prod )
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
            else if( dynamic_cast<Epsilon*>( token ) )
            {
                std::cout << "Epsilon\n";
                continue;
            }
            else
                std::cout << "ERROR: ";
            
            std::cout << "[" << token->raw() << "]"  << std::endl;
        }
        
        cout << endl;
    }

    return 0;
}
