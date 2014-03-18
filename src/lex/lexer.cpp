#include "lexer.h"

namespace lex {

/**
 *  Execute. 
 *  TODO:
 *  This should be replaced with a function that reads until it completes a Token,
 *  and this should call that until it reaches EOF.
 */
vector<Token> Lexer::lex( int fd )
{
    char c;
    Parser p;

    p.start();
    // Process loop.
    while( read( fd, &c, 1 ) != 0 )
    {
        if( is_plus( c ) )
            p.process_event( read_plus() );
        else if( is_minus( c ) )
            p.process_event( read_minus() );
        else if( is_number( c ) )
            p.process_event( read_number( c ) );
        else if( c == '\n' )
             break;
        else
        {
            cout << "Invalid char [" << c << "]. Exiting\n";
            break;
        }
    }

    p.stop();

    cout << "Found " << p.get_program().size() << " Tokens." << endl;

    return p.get_program();
}

} // End Namespace lex
