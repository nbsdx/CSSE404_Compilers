#ifndef TOKEN_H
#define TOKEN_H 1

#include <cstdlib>
#include <vector>
#include <string>
#include <sys/types.h>

using namespace std;

namespace lex {

/**
 *  Token class for the Lexer.
 */
class Token
{
public:
    enum TokenType { ID, Integer, ReservedWord, Operator, Delimiter };
    enum RWord { Class, Public, Static, Extends, Void, Int, Bool, If, Else, While,
                 Return, Null, True, False, This, New, String, Main, Println };
    enum OperType { Plus, Minus, Mult, Div, LT, LEq, GEq, GT, Eq, NEq, And, Or, Not };
    enum DelimType { Semi, Equal, LParen, RParen, LBrace, RBrace, LSquare, RSquare };

    /// Constructors 
    Token( enum TokenType t, int32_t v )        : type(t), intval(v) {}
    Token( enum TokenType t, enum RWord w )     : type(t), word(w) {}
    Token( enum TokenType t, string s )         : type(t), name(s) {}
    Token( enum TokenType t, enum OperType o )  : type(t), op(o) {}
    Token( enum TokenType t, enum DelimType d ) : type(t), dl(d) {}

    // Conveniece (spelling..) Functions
    static string delim_to_string( enum DelimType );
    static string rword_to_string( enum RWord );
    static string operator_to_string( enum OperType );

    // Display Functions
    string to_string() const;
  
    friend ostream& operator<<(ostream& os, const Token& tok);
//    static void display( vector<Token> );

protected:
    Token();
    TokenType       type;

    int32_t         intval;
    string          name;
    enum RWord      word;
    enum OperType   op;
    enum DelimType  dl;
};

} // End Namespace lex

#endif
