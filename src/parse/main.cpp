
#include <iostream>
#include <utility>
#include <vector>
#include "tt.hpp"

using namespace std;

typedef pair<string, vector<string>> Production;

int main()
{
    TransitionTable tt;
    vector<string>::iterator it;

    cout << tt.getNextProdIndex( 28, "Integer" ) << endl;
    cout << tt.getNextProdIndex( 17, "int" ) << endl;
    cout << tt.getNextProdIndex( 17, "boolean" ) << endl;
    cout << tt.getNextProdIndex( 17, "ID" ) << endl << endl;

    Production prod;
    prod = tt.getProduction( 10 );
    
    cout << "Prod: [" << prod.first << "]" << endl;
    for( it = prod.second.begin(); it != prod.second.end(); ++it )
    {
        cout << (*it) << " ";
    }
    cout << endl;

    return 0;
}
