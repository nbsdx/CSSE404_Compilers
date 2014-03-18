#include "token.h"
#include <iostream>
#include <string>

using namespace std;

namespace lex 
{
    ostream& operator<<( ostream& os, const Token &tok )
    {
        os << tok.to_string();
        return os;
    }

    string Token::to_string() const
    {
        switch(type) {
            case ID: 
                return string("ID, ").append( name );
            case Integer: 
                return string("Integer, ").append( std::to_string( intval ) );
            case ReservedWord: 
                return string("ReservedWord, ").append( rword_to_string( word ) );
            case Operator: 
                return string("Operator, ").append( operator_to_string( op ) );
            case Delimiter: 
                return string("Delimiter, ").append( delim_to_string( dl ) );
        }
        return string( "<<INVALID_TOKEN>>" );
    }

    string Token::rword_to_string( enum RWord r )
    {
        switch( r ) {
            case Class:     return string( "class" ); 
            case Public:    return string( "public" ); 
            case Static:    return string( "static" ); 
            case Extends:   return string( "extends" ); 
            case Void:      return string( "void" ); 
            case Int:       return string( "int" ); 
            case Bool:      return string( "boolean" ); 
            case If:        return string( "if" ); 
            case Else:      return string( "else" ); 
            case While:     return string( "while" ); 
            case Return:    return string( "return" ); 
            case Null:      return string( "null" ); 
            case True:      return string( "true" ); 
            case False:     return string( "false" ); 
            case This:      return string( "this" ); 
            case New:       return string( "new" ); 
            case String:    return string( "String" ); 
            case Main:      return string( "main" ); 
            case Println:   return string( "System.out.println" ); 
        }
        return string( "<<INVALID_RWORD>>" );
    }
    
    string Token::delim_to_string( enum DelimType d )
    {
        switch( d ) {
            case Semi:      return string( ";" );
            case LParen:    return string( "(" );
            case RParen:    return string( ")" );
            case LBrace:    return string( "{" );
            case RBrace:    return string( "}" );
            case LSquare:   return string( "[" );
            case RSquare:   return string( "]" );
            case Equal:     return string( "=" );
        }
        return string( "<<INVALID_DELIM>>" );
    }

    string Token::operator_to_string( enum OperType o )
    {
        switch( o ) {
            case Plus:      return string( "+" );
            case Minus:     return string( "-" );
            case Mult:      return string( "*" );
            case Div:       return string( "/" );
            case LT:        return string( "<" );
            case LEq:       return string( "<=" );
            case GEq:       return string( ">=" );
            case GT:        return string( ">" );
            case Eq:        return string( "==" );
            case NEq:       return string( "!=" );
            case And:       return string( "&&" );
            case Or:        return string( "||" );
            case Not:       return string( "!" );
        }
        return string( "<<INVALID_OPERATOR>>" );
    }
/*
    void Token::display( vector<Token> vec )
    {
        vector<Token>::iterator i;
        for( i = vec.begin(); i != vec.end(); ++i )
        {
            cout << (*i).to_string() << endl;
        }
    }
*/
} // End Namespace lex
