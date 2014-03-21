#include "lexer.h"

#define OPERCHARS "+-/!=><"
#define DELIMITERS ";[]{}()"
#define ISDIGIT(x) (x >= '0' && x <= '9')

namespace lex {

/**
 *  Execute. 
 *  Read one char from the stream, then dispatch another function
 *  that can complete the appropriate token.
 */
vector<Token> Lexer::lex( int fd )
{
    // Process loop.
    while( read( fd, &c, 1 ) != 0 )
    {
        if (c == ' ' || c == '\t' || c == '\n') {
            // Whitespace - ignore
            break;
        } else if (ISDIGIT(c)) {
            // Number function here
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

void Lexer::read_int(int fd, char *c) {
    if (!c) return; // oh no
    string num;
    num.append(1,*c);

    while( read(fd, c, 1) != 0) {
        if (ISDIGIT(*c)) {
            num.append(1, *c);
        }
    }
    // shift read head

    // Form integer
    int32_t read = atoi(num.c_str());
    Token tok = Token( Token::Integer, read );
    pgm.push_back(tok);
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
