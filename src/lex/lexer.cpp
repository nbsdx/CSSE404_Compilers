#include "lexer.h"

// NB: While = is a delimiter, it's handled in the operator function
//     (since this simplifies the == case)
#define OPERCHARS "+-/!=><&|"

#define DELIMITERS ";[]{}(),."

namespace lex {

char Lexer::read_one( int fd )
{
    int err;
    char input;

    err = read( fd, &input, 1 );

    if( err != 0 )
    {
        ++this->position;
        
        // Update line number.
        if( input == '\n' || input == '\r' )
        {
            ++this->linenumber;
            this->position = 0;
        }

        return input;
    }

    return 0;
}

void Lexer::backup( int fd, int amount )
{
    // Tries to fix position as much as possible...
    // TODO: Do this one char at a time to check for 
    // rewinding over newlines.
    lseek( fd, amount, SEEK_CUR );
    position += amount;
}

/**
 *  Execute. 
 *  Read one char from the stream, then dispatch another function
 *  that can complete the appropriate token.
 */
vector< shared_ptr<Token> > Lexer::lex( int fd )
{
    while( ( c = read_one( fd ) ) != 0 )
    {
        //cout << c << std::endl;
        if( c == ' ' || c == '\t' || c == '\n' || c == '\r' ) 
        {
            // Whitespace - ignore
            continue;
        } 
        else if( is_digit( c ) ) 
        {
            read_int( fd, c );
            continue;
        } 
        else if( is_alpha( c ) )
        {
            read_name( fd, c );
            continue;
        } 
        else if( is_delim_char( c ) )
        {
            read_delimiter( fd, c );
            continue;
        }
        else if( is_op_char( c ) )
        {
            read_operator( fd, c );
            continue;
        } 
        else 
        {
            // Fuck it, we're done.
            cerr << "Unexpected char [" << c << "]. Terminating.\n";
            break;
            // TODO: We should log invalid input.
        }
    }

    return pgm;
}

void Lexer::read_name( int fd, char c )
{
    bool has_period = false;
    string token;
    ReservedWord::RWord rword;
    char in;
    int err;

    token.append( 1, c );

    while( ( in = read_one( fd ) ) != 0 )
    {
        // Break conditions:
        if( !is_alpha( in ) && 
            !is_digit( in ) &&
            !( in == '_' ) )
        {
            if( in == '.' ) // Thanks Sys.out.ptln....
            {
                // check the string is System or System.out
                if( token.compare( "System" ) == 0  || 
                    token.compare( "System.out" ) == 0 )
                {
                    token.append( 1, in );
                    has_period = true;
                    continue;
                }
                else if( token.compare( "System.out.println" ) == 0 )
                {
                    // No stupid stuff. Just leave. We have it.
                    // This is to deal with the case of something like
                    // System.out.println.why.is.this.here
                    break;
                }
                else
                {
                    // Now we have to break it up, rewind to the 
                    // first . and clean up.
                    // If this is the first period that we've seen,
                    // we can just break, and it will be fine. 
                    // Otherwise we have to rewind our input.
                    if( has_period )
                        goto cleanup;
                    else 
                        break; // Don't need to explicitly state.
                }
            }

            // Stop input loop.
            break;
        }

        token.append( 1, in );
    }

    // God I wish we didn't have this shitty keyword. 
    // It makes everything more complicated. It should
    // probably just be parsed as a normal object, and 
    // the AST and parser can deal with it....
    if( has_period )
    {
        if( token.compare( "System.out.println" ) != 0 )
            goto cleanup;
    }

finalize:
    if( ( rword = ReservedWord::from_string( token ) ) == ReservedWord::Invalid_RWord )
        pgm.push_back( make_shared<Identifier>( token, linenumber, position ) );
    else
        pgm.push_back( make_shared<ReservedWord>( rword, linenumber, position ) );

    if( err != 0 )
        // Undo the last read.
        backup( fd, -1 );

    return;

    /**
     *  If this happens, then we tried to read a println and
     *  it wasn't actually a println. We need to figure out
     *  where the first '.' is, and then rewind that much, 
     *  but still add whatever was before it (System) as an ID.
     */
cleanup:
    int strlen = token.length();
    int first_p = token.find_first_of( '.' );

    token = token.substr( 0, first_p );

    //if( err != 0 )
        // This gets us to right after the '.'
        // The '.' rewound above.
        // There's something messed up with the is_one 
        // stuff that I think is breaking one of the 
        // testcases.... XXX
    backup( fd, -( strlen - first_p ) );

    goto finalize;
}

void Lexer::read_delimiter( int fd, char c )
{
    switch( c ) 
    {
        case ';': pgm.push_back( make_shared<Delimiter>( Delimiter::Semi, linenumber, position ) );
                  break;
        case '[': pgm.push_back( make_shared<Delimiter>( Delimiter::LSquare, linenumber, position ) );
                  break;
        case ']': pgm.push_back( make_shared<Delimiter>( Delimiter::RSquare, linenumber, position ) );
                  break;
        case '(': pgm.push_back( make_shared<Delimiter>( Delimiter::LParen, linenumber, position ) );
                  break;
        case ')': pgm.push_back( make_shared<Delimiter>( Delimiter::RParen, linenumber, position ) );
                  break;
        case '{': pgm.push_back( make_shared<Delimiter>( Delimiter::LBrace, linenumber, position ) );
                  break;
        case '}': pgm.push_back( make_shared<Delimiter>( Delimiter::RBrace, linenumber, position ) );
                  break;
        case ',': pgm.push_back( make_shared<Delimiter>( Delimiter::Comma, linenumber, position ) );
                  break;
        case '.': pgm.push_back( make_shared<Delimiter>( Delimiter::Period, linenumber, position ) );
                  break;
        default:  cerr << "Encounted Unexpected Delimiter " << c << std::endl;
                  break; // assert(false);
    }
}

void Lexer::read_operator( int fd, char c )
{
    // assert(c && is_one(OPERCHARS)
    int err;
    switch( c ) 
    {
        case '/': read_comdiv( fd );
                  break;
        case '+': pgm.push_back( make_shared<Operator>( Operator::Plus, linenumber, position ) );
                  break;
        case '-': pgm.push_back( make_shared<Operator>( Operator::Minus, linenumber, position ) );
                  break;
        case '*': pgm.push_back( make_shared<Operator>( Operator::Mult, linenumber, position ) );
                  break;
        case '<': read_twochar_operator( fd, '=', Operator::LT, Operator::LEq );
                  break;
        case '>': read_twochar_operator( fd, '=', Operator::GT, Operator::GEq );
                  break;
        case '!': read_twochar_operator( fd, '=', Operator::Not, Operator::NEqual );
                  break;
        case '&': {
                    char c1 = c;
                    char c2;

                    if( ( c2 = read_one( fd ) ) != 0 )
                    {
                        if( c2 == '&' )
                            pgm.push_back( make_shared<Operator>( Operator::And, linenumber, position ) );
                        else
                        {
                            cerr << "Encountered Unexpected character following a &." << endl;
                            backup( fd, -1 );
                        }
                    }
                  }
                  break;
        case '|': {
                    char c1 = c;
                    char c2;

                    if( ( c2 = read_one( fd ) ) != 0 )
                    {
                        if( c2 == '|' )
                            pgm.push_back( make_shared<Operator>( Operator::Or, linenumber, position ) );
                        else
                        {
                            cerr << "Encountered Unexpected character following a |." << endl;
                            backup( fd, -1 );
                        }
                    }
                  }
                  break;
        case '=': read_equal_assign( fd );
                  break;
        default:  cerr << "Encounted Unexpected Operator " << c << std::endl;
                  break;
    }
}

void Lexer::read_twochar_operator( int fd, char next, Operator::Op one, Operator::Op two ) 
{
    char c;
    c = read_one( fd );

    if( c == 0 )
        pgm.push_back( make_shared<Operator>( one, linenumber, position ) );
    else if( c != next ) 
    {
        // Oops, read too much.
        backup( fd, -1 );
        pgm.push_back( make_shared<Operator>( one, linenumber, position ) );
    } 
    else 
    {
        pgm.push_back( make_shared<Operator>( two, linenumber, position ) );
    }
}

void Lexer::read_equal_assign( int fd ) 
{
    char c = read_one( fd );

    if( c == 0 || c != '=' )
    {
        // Opps, not a ==, just a =.
        backup( fd, -1 );
        pgm.push_back( make_shared<Delimiter>( Delimiter::Equal, linenumber, position ) );
    }
    else 
    {
        pgm.push_back( make_shared<Operator>( Operator::EqualEq, linenumber, position ) );
    }
}
/*
void Lexer::maybe_read_twochar( int fd, char next, Operator::Op just ) {
    char c;
    int err = read( fd, &c, 1);
    if (err != 0 && c != next) 
    {
        // TODO: Decide on plan for error handling
        lseek( fd, -1, SEEK_CUR);
        pgm.push_back( make_shared<Operator>( Operator::Invalid_Op ));
    } 
    else 
    {
        pgm.push_back( make_shared<Operator>( just ));
    }
}
*/
void Lexer::read_comdiv( int fd ) 
{
    // assert (c && *c == '/')
    
    char c = read_one( fd );

    if( c != 0 && c == '/' ) 
    {
        return comm_line( fd );
    } 
    else if( c != 0 && c == '*' ) 
    {
        return comm_block( fd );
    } 
    else
    {
        pgm.push_back( make_shared<Operator>( Operator::Div, linenumber, position ) );
    }
    if( c != 0 ) 
    {
        // (and we just ate another function's character)
        backup( fd, -1 );
    }
}

void Lexer::comm_line( int fd ) 
{
    char c;

    while ( ( c = read_one( fd ) ) != 0 ) 
    {
        if( c == 0 )
            break;
        else if( c == '\n' || c == '\r' )
            break;
    }
}

void Lexer::comm_block( int fd )
{
    int err;
    char cur = 0;
    char last = '\0';
    
    while( ( cur = read_one( fd ) ) != 0 )
    {
        if( last == '*' && cur == '/' )
            break;

        last = cur;
    }
}

void Lexer::read_int( int fd, char c )
{
    int err;
    string num;

    num.append( 1, c );

    // If the first character is 0, then the
    // 0 is it's own Integer.
    if( c == '0' )
        goto make_token;

    while( ( c = read_one( fd ) ) != 0 )
    {
        if( c == 0 )
            break;
        else if( is_digit( c ) )
            num.append( 1, c );
        else
        {
            backup( fd, -1 );
            break;
        }
    }

make_token:
    // Form integer
    pgm.push_back( make_shared<Number>( stoi( num ), linenumber, position ) );
}

// Accept a string literal (like a #define) and check char membership
bool Lexer::is_one(char candidate, const char* group) {
    const char* g; 
    bool ret = false;

    for (g = group; g != NULL; g++) {
        if (*g == candidate) {
            ret = true;
            break;
        }
    }
    return ret;
}

} // End Namespace lex
