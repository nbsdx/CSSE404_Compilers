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
            if (err == 0) break;
        }

        if (c == ' ' || c == '\t' || c == '\n') {
            // Whitespace - ignore
            break;
        } else if (ISDIGIT(c)) {
            // Number function here
            status = read_int(fd, &c);
        } else if (c >= 'a' && c <= 'Z') {
            // Name function
        } else if (is_one(c, OPERCHARS)) {
            // Operator / comment function
        } else if (is_one(c, DELIMITERS)) {
            // Delimiter switch
        } else {
            cout << "Invalid char [" << c << "]. Exiting\n";
        }
    }

    return pgm;
}

int Lexer::read_int(int fd, char *c) {
    // assert(c)
    int err;
    int ret = CLEAN;
    string num;
    num.append(1,*c);

    while( true ) {
        err = read(fd, c, 1);
        if (err == 0) {
            ret = EOF_T;
        } else if (ISDIGIT(*c)) {
            num.append(1, *c);
        } else {
            ret = DIRTY;
        }
    }

    // Form integer
    int32_t read = atoi(num.c_str());
    Token tok = Token( Token::Integer, read );
    pgm.push_back(tok);
    return ret;
}

// Accept a string literal (like a #define) and check char membership
bool is_one(char candidate, const char* group) {
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
