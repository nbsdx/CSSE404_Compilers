#include "lexer.h"

#define OPERCHARS "+-/!=><&|"
#define DELIMITERS ";[]{}()"
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
        if (c == ' ' || c == '\t' || c == '\n') 
        {
            // Whitespace - ignore
        } 
        else if (ISDIGIT(c)) 
        {
            // Number function here
            read_int(fd, &c);
        } 
        else if ( is_alpha( c ) ) 
        {
            // Name function
            read_name( fd );
        } 
        else if (is_one(c, OPERCHARS)) 
        {
            // Operator / comment function
            read_operator(fd, &c);
        } 
        else if (is_one(c, DELIMITERS)) 
        {
            // Delimiter switch
            cout << "Delimiter, " << c << std::endl;
        } 
        else 
        {
            cout << "Invalid char [" << c << "]. Ignoring.\n";
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

    // Go back 1 character... This is ghetto.
    lseek( fd, -1, SEEK_CUR );

    while( ( read( fd, &in, 1 ) ) != 0 )
    {
        // Break conditions:
        if( !is_alpha( in ) )
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
        pgm.push_back( make_shared<Identifier>( token ) );
    else
        pgm.push_back( make_shared<ReservedWord>( rword ) );

    // Undo the last read.
    lseek( fd, -1, SEEK_CUR );


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

    // This gets us to right after the '.'
    // The '.' rewound above.
    // There's something messed up with the is_one 
    // stuff that I think is breaking one of the 
    // testcases.... XXX
    lseek( fd, -( strlen - first_p + 1 ), SEEK_CUR );

    goto finalize;
}

void Lexer::read_operator(int fd, char *c) {
    // assert(c && is_one(OPERCHARS)
    int err;
    switch (*c) {
        case '/': return read_comdiv(fd, c);
        case '+': pgm.push_back( make_shared<Operator>( Operator::Plus) );
                  break;
        case '-': pgm.push_back( make_shared<Operator>( Operator::Minus ) );
                  break;
        case '*': pgm.push_back( make_shared<Operator>( Operator::Mult ));
                  break;
        case '<': return read_twochar_operator(fd, '=', Operator::LT, Operator::LEq);
        case '>': return read_twochar_operator(fd, '=', Operator::GT, Operator::GEq);
        case '!': return read_twochar_operator(fd, '=', Operator::Not, Operator::NEqual);
        case '&': return maybe_read_twochar(fd, '&', Operator::And);
        case '|': return maybe_read_twochar(fd, '|', Operator::Or);
        case '=': return read_equal_assign(fd);
    }
}

void Lexer::read_twochar_operator(int fd, char next, Operator::Op one, Operator::Op two) {
    char c;
    int err = read( fd, &c, 1);
    if (err != 0 && c != next) {
        lseek( fd, -1, SEEK_CUR);
    }
    if (c != next || err == 0) { // urgh
        pgm.push_back( make_shared<Operator>( one ));
    } else {
        pgm.push_back( make_shared<Operator>( two));
    }
}

void Lexer::read_equal_assign(int fd) {
    char c;
    int err = read( fd, &c, 1 );
    if (err == 0 || c != '=') {
        pgm.push_back( make_shared<Delimiter>( Delimiter::Equal ));
        lseek( fd, -1, SEEK_CUR);
    } else {
        pgm.push_back( make_shared<Operator>( Operator::EqualEq ));
    }
}

void Lexer::maybe_read_twochar(int fd, char next, Operator::Op just) {
    char c;
    int err = read( fd, &c, 1);
    if (err != 0 && c != next) {
        // TODO: Decide on plan for error handling
        pgm.push_back( make_shared<Operator>( Operator::Invalid_Op ));
        lseek( fd, -1, SEEK_CUR);
    } else {
        pgm.push_back( make_shared<Operator>( just ));
    }
}

void Lexer::read_comdiv(int fd, char *c) {
    // assert (c && *c == '/')
    int err = read( fd, c, 1 );
    if (err == 0) {
        // We have a div!
        //
    } else if (*c == '/') {
        return comm_line(fd, c);
    } else if (*c == '*') {
        return comm_block(fd, c);
    }
    // In all other cases we have a Div operator
    // (and we just ate another function's character)
    lseek( fd, -1, SEEK_CUR);
    pgm.push_back( make_shared<Operator>( Operator::Div ) );
}

void Lexer::comm_line(int fd, char *c) {
    int err;
    while ((err = read( fd, c, 1 ))) {
        if (err == 0) {
            break;
        } else if (*c == '\n') {
            break;
        }
    }
}

void Lexer::comm_block(int fd, char *c) {
    int err;
    char last = '\0';
    while ((err = read(fd, c, 1))) {
        if (err == 0) {
            break;
        } else if (last == '*' && *c == '/') {
            break;
        }
        last = *c;
    }
}

void Lexer::read_int(int fd, char *c) {
    // assert(c && ISDIGIT(*c)
    int err;
    string num;
    num.append(1,*c);
    while(( err = read(fd, c, 1))){
        if (err == 0) {
            break;
        } else if (ISDIGIT(*c)) {
            num.append(1, *c);
        } else {
            lseek( fd, -1, SEEK_CUR );
            break;
        }
    }

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
