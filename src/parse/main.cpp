
#include <iostream>
#include <utility>
#include <vector>
#include "tt.hpp"

using namespace std;

int main()
{
    // Initialize our Transition Table.
    TransitionTable tt;
    vector<shared_ptr<Token>>::iterator it;

    // Test some transitions
    cout << tt.getNextProdIndex( 29, "Integer" ) << endl;
    cout << tt.getNextProdIndex( 17, "int" ) << endl;
    cout << tt.getNextProdIndex( 17, "boolean" ) << endl;
    cout << tt.getNextProdIndex( 17, "ID" ) << endl << endl;

    // Get a production to test.
    Production prod = tt.getProduction( 10 );
    
    cout << "Prod: [" << prod.lhs << "]" << endl;
    for( it = prod.rhs->begin(); it != prod.rhs->end(); ++it )
    {
        cout << (*it)->format() << endl;
    }
    cout << endl;

    return 0;
}
