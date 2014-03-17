//  Simple types for Token representation.

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include <sys/types.h>
#include <string.h>

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
        std::stringstream sstm;
        switch(type) {
            case ID: sstm << "ID, " << name;
            case Integer: sstm << "Integer, " << intval;
            case ReservedWord: sstm << "ReservedWord, " << rword_str(word);
            case Operator: sstm << "Operator, " << oper_str(op);
            case Delimiter: sstm << "Delimiter, "; sstm.put(delim_char(dl));
        }
        return sstm.str();
    }

private:
    Token ();
    int32_t intval;
    std::string name;
    enum RWord word;
    enum OperType op;
    enum DelimType dl;

    // Laborious work just for the pretty printer
    std::string oper_str(enum OperType op) {
        std::stringstream sstm;
        switch(op) {
            case Plus: sstm << "plus";
            case Minus: sstm << "minus";
            // TODO: Fill out cases. Not sure of format
            // Mult, Div, LT, LEq, GEq, GT, Eq, NEq, And, Or, Not
        }
        return sstm.str();
    }

    std::string rword_str(enum RWord w) {
        std::stringstream sstm;
        switch(w) {
            case Class: sstm << "class";
            case Public: sstm << "public";
            case Static: sstm << "static";
            case Extends: sstm << "extends";
            case Void: sstm << "void";
            case Int: sstm << "int";
            case Bool: sstm << "bool";
            case If: sstm << "if";
            case Else: sstm << "else";
            case While: sstm << "while";
            case Return: sstm << "return";
            case Null: sstm << "null";
            case True: sstm << "true";
            case False: sstm << "false";
            case This: sstm << "this";
            case New: sstm << "new";
            case String: sstm << "string";
            case Main: sstm << "main";
            case Println: sstm << "System.out.println";
        }
        return sstm.str();
    }

    char delim_char(enum DelimType dl) {
        switch(dl) {
        // Semi, LParen, RParen, LBrace, RBrace, LSquare, RSquare 
            case Semi:     return ';';
            case LParen:   return '(';
            case RParen:   return ')';
            case LBrace:   return '{';
            case RBrace:   return '}';
            case LSquare:  return '[';
            case RSquare:  return ']';
        }
    }

};

void print_pgm (std::vector<Token> pgm) {
    std::vector<Token>::iterator iter;
    for (iter = pgm.begin(); iter != pgm.end(); iter++) {
        std::string str = (*iter).print_repr();
        std::cout << str << '\n'; // Why doesn't this work?!!!
    }
}


