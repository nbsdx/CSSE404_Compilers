#ifndef LEXER_H
#define LEXER_H 1

#include "token.h"
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

namespace lex {

class Lexer
{
public:
    vector< shared_ptr<Token> > lex( int fd );
protected:
private:
    char c;
    vector< shared_ptr<Token> > pgm;

    uint32_t linenumber;
    uint32_t position;

    /* IO functions */
    char read_one( int fd );
    void backup( int fd, 
                 int amount );

    /* Top Level Machines */
    void read_int      ( int fd, char c );
    void read_delimiter( int fd, char c );
    void read_operator ( int fd, char c );
    void read_name     ( int fd, char c );

    /* Submachines */
    void read_comdiv      ( int fd );
    void comm_line        ( int fd );
    void comm_block       ( int fd );
    void read_equal_assign( int fd );
    void read_twochar_operator( int fd, 
                                char next, 
                                Operator::Op one, 
                                Operator::Op two );

    inline bool is_alpha( char c )
        { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
    
    inline bool is_op_char( char c )
        { return std::string("+-*/!=<>&|").find_first_of( c ) != string::npos; }
    
    inline bool is_delim_char( char c )
        { return std::string(";[]{}(),.").find_first_of( c ) != string::npos; }

    inline bool is_digit( char c )
        { return ( c >= '0' && c <= '9' ); }
  
    bool is_one( char candidate, const char* group );

//    void maybe_read_twochar(int fd, char next, Operator::Op just);
};

} // End Namespace lex

#endif
