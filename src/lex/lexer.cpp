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
vector<Token> Lexer::lex( int fd )
{

    int status = CLEAN;
    int err = 0;
    while( status != EOF_T )
    {
        if (status == CLEAN) {
            err = read( fd, &c, 1 );
            if (err == 0) {
                status = EOF_T;
                break;
            }
        }

        // Else reuse old c
        status = CLEAN;

        if (c == ' ' || c == '\t' || c == '\n') {
            // Whitespace - ignore
        } else if (ISDIGIT(c)) {
            // Number function here
            status = read_int(fd, &c);
        } else if (c >= 'a' && c <= 'Z') {
            // Name function
        } else if (is_one(c, OPERCHARS)) {
            // Operator / comment function
            status = read_operator(fd, &c);
        } else if (is_one(c, DELIMITERS)) {
            // Delimiter switch
        } else {
            cout << "Invalid char [" << c << "]. Exiting\n";
        }
    }

    return pgm;
}

int Lexer::read_operator(int fd, char *c) {
    // assert(c && is_one(OPERCHARS)
    int err;
    int ret = CLEAN;
    switch (*c) {
        case '/': return read_comdiv(fd, c);
        case '+': pgm.push_back(Token( Token::Operator, Token::Plus ));
                  break;
        case '-': pgm.push_back(Token( Token::Operator, Token::Minus ));
                  break;

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
    pgm.push_back(Token( Token::Operator, Token::Div));
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
    int32_t read = atoi(num.c_str());
    Token tok = Token( Token::Integer, read );
    pgm.push_back(tok);
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
