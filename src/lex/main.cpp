#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <memory>

#include "lexer.h"

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
        std::cout << "Error opening file" << std::endl; //<< strerror( errno ) << "\n";
        exit( 1 );
    }

    std::vector<BasicToken*> pgm = Lexer().lex( fd );

//    std::cout << std::endl;

    for( BasicToken *token : pgm )
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
        else
            std::cout << "ERROR: ";

        std::cout << token->raw() << std::endl;
    }
    return 0;
}
