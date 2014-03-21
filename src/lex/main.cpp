#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>

#include "lexer.h"
#include "token.h"

using namespace lex;

int main( int argc, char **argv )
{
    int fd;

    // Argument validation
    if( argc != 2 )
    {
        std::cout << "Please suppy the input file as arg 1.\n"; 
        exit( 1 );
    }

    // File open
    if( ( fd = open( argv[1], O_RDONLY ) ) == -1 )
    {
        std::cout << "Error opening file: " << strerror( errno ) << "\n";
        exit( 1 );
    }

    std::vector<Token> pgm = Lexer().lex( fd );

    std::vector<Token>::iterator i;
    for( i = pgm.begin(); i != pgm.end(); ++i )
        cout << (*i).to_string() << std::endl;

    return 0;
}
