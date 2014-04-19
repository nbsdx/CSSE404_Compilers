
#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include "tt.hpp"
#include "../lex/lexer.h"
#include "AST.h"
#include "context.h"
#include "typecheck.h"
#include "parse.h"

using namespace std;

int main( int argc, char **argv )
{
    if( argc < 2 )
    {
        std::cout << "Error please supply file\n";
        exit( 1 );
    }

    ifstream file( argv[1], ifstream::binary );
    if( !file )
//    if( ( fd = open( argv[1], O_RDONLY ) ) == -1 )
    {
        std::cout << "Error opening file\n";
        exit( 1 );
    }

    // Lexer Output.
    //vector<BasicToken*> pgm = lex::Lexer().lex( fd );
    vector<BasicToken*> pgm = lex::Lexer().lex( file );

    Parser p;

    RTree *raw = p.parse( pgm );


    TypeCheck *tc = new TypeCheck();

    RTree *checked = tc->check( raw );

    if (tc->clean) {
        cerr << "Passed type checking. Compilatioon can proceed.\n";
    } else {
        cerr << "Type errors detected. Compilation must halt.\n";
    }

    // holy duplication
    if( ( argc > 2 ) && ( string( "--print" ).compare( argv[2] ) == 0 ) )
    {
        checked->printT();
        cout << endl;
    } else cerr << "Run with --print to see AST.\n";


    if (tc->clean) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}