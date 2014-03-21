#include "lexer.h"

#define OPERCHARS "+-/!=><"
#define DELIMITERS ";[]{}()"
#define ISDIGIT(x) (x >= '0' && x <= '9')

// Is the char on top of the stream to be processed?
#define CLEAN 0
#define DIRTY 1
#define EOF_T 2

namespace lex {

/**
 *  Execute. 
 *  Read one char from the stream, then dispatch another function
 *  that can complete the appropriate token.
 */
vector< shared_ptr<Token> > Lexer::lex( int fd )
{
    int status = CLEAN;
    int err = 0;
    while( status != EOF_T )
    {
        if (status == CLEAN) 
        {
            err = read( fd, &c, 1 );
            if (err == 0) {
                status = EOF_T;
                break;
            }
        }

        status = CLEAN;

        if (c == ' ' || c == '\t' || c == '\n') 
        {
            // Whitespace - ignore
        } 
        else if (ISDIGIT(c)) 
        {
            // Number function here
            status = read_int(fd, &c);
        } 
        else if ( is_alpha( c ) ) 
        {
            // Name function
            status = read_name( fd );
        } 
        else if (is_one(c, OPERCHARS)) 
        {
            // Operator / comment function
            status = read_operator(fd, &c);
        } 
        else if (is_one(c, DELIMITERS)) 
        {
            // Delimiter switch
        } 
        else 
        {
            cout << "Invalid char [" << c << "]. Ignoring.\n";
            // TODO: We should log invalid input.
        }
    }

    return pgm;
}

int Lexer::read_name( int fd )
{
    string token;
    ReservedWord::RWord rword;
    char in;
    int ret = CLEAN;
    
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
                if( token.compare( "System" )     == 0 ||
                    token.compare( "System.out" ) == 0 )
                {
                    token.append( 1, in );
                    continue;
                }
            }
            break;
        }

        token.append( 1, in );
    }
    
    if( ( rword = ReservedWord::from_string( token ) ) == ReservedWord::Invalid_RWord )
        pgm.push_back( make_shared<Identifier>( token ) );
    else
        pgm.push_back( make_shared<ReservedWord>( rword ) );
    
    // Undo the last read.
    lseek( fd, -1, SEEK_CUR );

    return ret;
}

int Lexer::read_operator(int fd, char *c) {
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
        case '&': break; //return read_twochar_operator(fd, '&', Token::BAnd, Token::And);
        case '|': // TODO: As above for || or ERROR (boolops)
                  break;
        case '=': // TODO: == vs delimiter =
                  break;
    }
    return CLEAN;
}

int Lexer::read_twochar_operator(int fd, char next, Operator::Op one, Operator::Op two) {
    int ret;
    char c;
    int err = read( fd, &c, 1);
    if (err == 0) ret = EOF_T;
    if (c != next) ret = DIRTY;
    if (c != next || err == 0) { // urgh
        pgm.push_back( make_shared<Operator>( one ));
    } else {
        pgm.push_back( make_shared<Operator>( two));
    }
    return ret;
}

int Lexer::read_comdiv(int fd, char *c) {
    // assert (c && *c == '/')
    int ret = CLEAN;
    int err = read( fd, c, 1 );
    if (err == 0) {
        // We have a div!
        ret = EOF_T;
    } else if (*c == '/') {
        return comm_line(fd, c);
    } else if (*c == '*') {
        return comm_block(fd, c);
    }
    // In all other cases we have a Div operator
    pgm.push_back( make_shared<Operator>( Operator::Div ) );
    return ret;
}

int Lexer::comm_line(int fd, char *c) {
    int err;
    int ret = CLEAN;
    while ((err = read( fd, c, 1 ))) {
        if (err == 0) {
            ret = EOF_T;
            break;
        } else if (*c == '\n') {
            break;
        }
    }
    return CLEAN;
}

int Lexer::comm_block(int fd, char *c) {
    int err;
    int ret = CLEAN;
    char last = '\0';
    while ((err = read(fd, c, 1))) {
        if (err == 0) {
            ret = EOF_T;
            break;
        } else if (last == '*' && *c == '/') {
            break;
        }
    }
    return CLEAN;
}

int Lexer::read_int(int fd, char *c) {
    // assert(c && ISDIGIT(*c)
    int err;
    int ret = CLEAN;
    string num;
    num.append(1,*c);
    while(( err = read(fd, c, 1))){
        if (err == 0) {
            ret = EOF_T;
            break;
        } else if (ISDIGIT(*c)) {
            num.append(1, *c);
        } else {
            ret = DIRTY;
            break;
        }
    }

    // Form integer
    pgm.push_back( make_shared<Number>( stoi( num ) ) );
    return ret;
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
