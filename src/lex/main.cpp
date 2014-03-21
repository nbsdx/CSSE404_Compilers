#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <memory>

#include <error.h>
#include <string.h>

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

    std::vector< shared_ptr<Token> > pgm = Lexer().lex( fd );

    for( auto token : pgm )
        std::cout << token->format() << std::endl;

    return 0;
}
