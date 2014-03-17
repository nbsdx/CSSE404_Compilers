#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string>

#include "lexer.h"

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

    std::vector<lex::Token> pgm = lex::Lexer().lex( fd );
    lex::Token::display( pgm );

    return 0;
}
