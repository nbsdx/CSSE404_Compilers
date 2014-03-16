//  Simple types for Token representation.

#include <cstdlib>
#include <sys/types.h>
#include <string.h>
#include <string>

enum TokenType { ID, Integer, ReservedWord, Operator, Delimiter };
enum RWord { Class, Public, Static, Extends, Void, Int, Bool, If, Else, While,
             Return, Null, True, False, This, New, String, Main, Println };
enum OperType { Plus, Minus, Mult, Div, LT, LEq, GEq, GT, Eq, NEq, And, Or, Not };
enum DelimType { Semi, LParen, RParen, LBrace, RBrace, LSquare, RSquare };

class Token {
public:
    enum TokenType type;

    // Constructors
    Token(enum TokenType t, int32_t v) {
        type = t;
        intval = v;
    }
    Token(enum TokenType t, enum RWord w) {
        type = t;
        word = w;
    };
    Token(enum TokenType t, std::string n) {
        type = t;
        name = n;
    };
    Token(enum TokenType t, enum OperType o) {
        type = t;
        op = o;
    };
    Token(enum TokenType t, enum DelimType d) {
        type = t;
        d = dl;
    };

    std::string print_repr() {
        switch(type) {
            // TODO: Actually write this function
            case ID: return std::string("ID");
            case Integer: return std::string("Integer, ");
            case ReservedWord: return std::string("ReservedWord, ");
            case Operator: return std::string("Operator, ");
            case Delimiter: return std::string("Delimiter, ");
        }
    }

private:
    Token ();
    int32_t intval;
    std::string name;
    enum RWord word;
    enum OperType op;
    enum DelimType dl;
};
