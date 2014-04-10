#ifndef LEXER_H
#define LEXER_H 1

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include "../util/token.h"

using namespace std;

namespace lex {

class Lexer
{
public:
    vector<BasicToken*> lex( ifstream &file );
protected:
private:
    char c;
    vector<BasicToken*> pgm;

    uint32_t linenumber;
    uint32_t position;

    /* IO functions */
    char read_one( ifstream &file );
    void backup( ifstream &file, 
                 int amount );

    /* Top Level Machines */
    void read_int      ( ifstream &file, char c );
    void read_delimiter( ifstream &file, char c );
    void read_operator ( ifstream &file, char c );
    void read_name     ( ifstream &file, char c );

    /* Submachines */
    void read_comdiv      ( ifstream &file );
    void comm_line        ( ifstream &file );
    void comm_block       ( ifstream &file );
    void read_equal_assign( ifstream &file );
    void read_twochar_operator( ifstream &file, 
                                char next, 
                                Operators one, 
                                Operators two );

    inline bool is_alpha( char c )
        { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
    
    inline bool is_op_char( char c )
        { return std::string("+-*/!=<>&|").find_first_of( c ) != string::npos; }
    
    inline bool is_delim_char( char c )
        { return std::string(";[]{}(),.").find_first_of( c ) != string::npos; }

    inline bool is_digit( char c )
        { return ( c >= '0' && c <= '9' ); }
  
    bool is_one( char candidate, const char* group );

//    void maybe_read_twochar(ifstream &file, char next, Operator::Op just);
};

} // End Namespace lex

#endif
