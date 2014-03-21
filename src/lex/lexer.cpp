#include "lexer.h"

// NB: While = is a delimiter, it's handled in the operator function
//     (since this simplifies the == case)
#define OPERCHARS "+-/!=><&|"

#define DELIMITERS ";[]{}(),."

#define ISDIGIT(x) (x >= '0' && x <= '9')

namespace lex {

/**
 *  Execute. 
 *  Read one char from the stream, then dispatch another function
 *  that can complete the appropriate token.
 */
vector< shared_ptr<Token> > Lexer::lex( int fd )
{
    while(read( fd, &c, 1) != 0)
    {
        //cout << c << std::endl;
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') 
        {
            // Whitespace - ignore
            continue;
        } 
        else if (ISDIGIT(c)) 
        {
            read_int(fd, &c);
            continue;
        } 
        else if ( is_alpha( c ) ) 
        {
            read_name( fd );
            continue;
        } 
        else if ( is_delim_char( c ) )
        {
            read_delimiter(fd, &c);
            continue;
        }
        else if ( is_op_char( c ) )
        {
            read_operator(fd, &c);
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

void Lexer::read_name( int fd )
{
    bool has_period = false;
    string token;
    ReservedWord::RWord rword;
    char in;
    int err;

    // Go back 1 character... This is ghetto.
    lseek( fd, -1, SEEK_CUR );

    while( ( err = read( fd, &in, 1 ) ) != 0 )
    {
        // Break conditions:
        if( !is_alpha( in ) && 
            !ISDIGIT( in ) )
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

            // Fix for Test Case A7-03. It turns "m_name" into:
            // ID, m
            // ID, name
            // so we want to consume the _; do this by setting err to 0.
            if( in == '_' )
                err = 0;

            break;
        }

        token.append( 1, in );

        if( token.compare( "System.out.println" ) == 0 )
        {
            read( fd, &in, 1 );
            break;
        }
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
        pgm.push_back( make_shared<Identifier>( token ) );
    else
        pgm.push_back( make_shared<ReservedWord>( rword ) );

    if( err != 0 )
        // Undo the last read.
        lseek( fd, -1, SEEK_CUR );

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

    if( err != 0 )
        // This gets us to right after the '.'
        // The '.' rewound above.
        // There's something messed up with the is_one 
        // stuff that I think is breaking one of the 
        // testcases.... XXX
        lseek( fd, -( strlen - first_p ), SEEK_CUR );

    goto finalize;
}

void Lexer::read_delimiter(int fd, char *c) {
    switch (*c) 
    {
        case ';': pgm.push_back( make_shared<Delimiter>( Delimiter::Semi ));
                  break;
        case '[': pgm.push_back( make_shared<Delimiter>( Delimiter::LSquare ));
                  break;
        case ']': pgm.push_back( make_shared<Delimiter>( Delimiter::RSquare ));
                  break;
        case '(': pgm.push_back( make_shared<Delimiter>( Delimiter::LParen ));
                  break;
        case ')': pgm.push_back( make_shared<Delimiter>( Delimiter::RParen ));
                  break;
        case '{': pgm.push_back( make_shared<Delimiter>( Delimiter::LBrace ));
                  break;
        case '}': pgm.push_back( make_shared<Delimiter>( Delimiter::RBrace ));
                  break;
        case ',': pgm.push_back( make_shared<Delimiter>( Delimiter::Comma ));
                  break;
        case '.': pgm.push_back( make_shared<Delimiter>( Delimiter::Period ));
                  break;
        default:  cerr << "Encounted Unexpected Delimiter " << *c << std::endl;
                  break; // assert(false);
    }
}

void Lexer::read_operator(int fd, char *c) {
    // assert(c && is_one(OPERCHARS)
    int err;
    switch (*c) {
        case '/': read_comdiv(fd, c);
                  break;
        case '+': pgm.push_back( make_shared<Operator>( Operator::Plus) );
                  break;
        case '-': pgm.push_back( make_shared<Operator>( Operator::Minus ) );
                  break;
        case '*': pgm.push_back( make_shared<Operator>( Operator::Mult ));
                  break;
        case '<': read_twochar_operator(fd, '=', Operator::LT, Operator::LEq);
                  break;
        case '>': read_twochar_operator(fd, '=', Operator::GT, Operator::GEq);
                  break;
        case '!': read_twochar_operator(fd, '=', Operator::Not, Operator::NEqual);
                  break;
        case '&': {
                    char c1 = *c;
                    char c2;

                    if( read( fd, &c2, 1 ) != 0 )
                    {
                        if( c2 == '&' )
                            pgm.push_back( make_shared<Operator>( Operator::And ) );
                        else
                        {
                            cerr << "Encountered Unexpected character following a &." << endl;
                            lseek( fd, -1, SEEK_CUR );
                        }
                    }
                  }
                  break;
        case '|': {
                    char c1 = *c;
                    char c2;

                    if( read( fd, &c2, 1 ) != 0 )
                    {
                        if( c2 == '|' )
                            pgm.push_back( make_shared<Operator>( Operator::Or ) );
                        else
                        {
                            cerr << "Encountered Unexpected character following a |." << endl;
                            lseek( fd, -1, SEEK_CUR );
                        }
                    }
                  }
                  break;
        case '=': read_equal_assign(fd);
                  break;
        default:  cerr << "Encounted Unexpected Operator " << *c << std::endl;
                  break;
    }
}

void Lexer::read_twochar_operator(int fd, char next, Operator::Op one, Operator::Op two) {
    char c;
    int err = read( fd, &c, 1 );

    if( err == 0 )
        pgm.push_back( make_shared<Operator>( one ) );
    else if( c != next ) 
    {
        // Oops, read too much.
        lseek( fd, -1, SEEK_CUR );
        pgm.push_back( make_shared<Operator>( one ));
    } 
    else 
    {
        pgm.push_back( make_shared<Operator>( two));
    }
}

void Lexer::read_equal_assign( int fd ) {
    char c;
    int err = read( fd, &c, 1 );
    if (err == 0 || c != '=') 
    {
        // Opps, not a ==, just a =.
        lseek( fd, -1, SEEK_CUR );
        pgm.push_back( make_shared<Delimiter>( Delimiter::Equal ) );
    }
    else 
    {
        pgm.push_back( make_shared<Operator>( Operator::EqualEq ) );
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
void Lexer::read_comdiv(int fd, char *c) {
    // assert (c && *c == '/')
    
    int err = read( fd, c, 1 );

    if (err != 0 && *c == '/') 
    {
        return comm_line( fd, c );
    } 
    else if (err != 0 && *c == '*') 
    {
        return comm_block( fd, c );
    } 
    else
    {
        pgm.push_back( make_shared<Operator>( Operator::Div ) );
    }
    if (err != 0) 
    {
        // (and we just ate another function's character)
        lseek( fd, -1, SEEK_CUR );
    }
}

void Lexer::comm_line(int fd, char *c) {
    int err;

    while ((err = read( fd, c, 1 ))) 
    {
        if (err == 0)
            break;
        else if (*c == '\n' || *c == '\r')
            break;
    }
}

void Lexer::comm_block(int fd, char *c) {
    int err;
    char last = '\0';
    
    while (read(fd, c, 1) != 0) 
    {
        if (last == '*' && *c == '/')
            break;

        last = *c;
    }
}

void Lexer::read_int(int fd, char *c) {
    // assert(c && ISDIGIT(*c)
    int err;
    string num;
    num.append(1,*c);

    // If the first character is 0, then the
    // 0 is it's own Integer.
    if( *c == '0' )
        goto make_token;

    while(( err = read(fd, c, 1)))
    {
        if (err == 0)
            break;
        else if (ISDIGIT(*c))
            num.append(1, *c);
        else
        {
            lseek( fd, -1, SEEK_CUR );
            break;
        }
    }

make_token:
    // Form integer
    pgm.push_back( make_shared<Number>( stoi( num ) ) );
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
