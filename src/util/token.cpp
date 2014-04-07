
#include "token.h"

namespace Util {

ReservedWords RWord_FromString( const std::string &str )
{
    switch( _hash( str.c_str() ) )
    {
		case _hash( "class" ):  return ReservedWords::Class;
		case _hash( "public" ): return ReservedWords::Public;
		case _hash( "static" ): return ReservedWords::Static;
		case _hash( "extends" ):return ReservedWords::Extends;
		case _hash( "void" ):   return ReservedWords::Void;
		case _hash( "int" ):    return ReservedWords::Int;
		case _hash( "boolean" ):return ReservedWords::Bool;
		case _hash( "if" ):     return ReservedWords::If;
		case _hash( "else" ):   return ReservedWords::Else;
		case _hash( "while" ):  return ReservedWords::While;
		case _hash( "return" ): return ReservedWords::Return;
		case _hash( "null" ):   return ReservedWords::Null;
		case _hash( "true" ):   return ReservedWords::True;
		case _hash( "false" ):  return ReservedWords::False;
		case _hash( "this" ):   return ReservedWords::This;
		case _hash( "new" ):    return ReservedWords::New;
		case _hash( "String" ): return ReservedWords::String;
		case _hash( "main" ):   return ReservedWords::Main;
		case _hash( "System.out.println" ): return ReservedWords::Println;
        default: return ReservedWords::Invalid_RWord;
    }
}

Operators Operator_FromString( const std::string &str )
{
    switch( _hash( str.c_str() ) )
    {
        case _hash( "+" ): return Operators::Plus;
        case _hash( "-" ): return Operators::Minus;
        case _hash( "*" ): return Operators::Mult;
        case _hash( "/" ): return Operators::Div;
        case _hash( "<" ): return Operators::LT;
        case _hash( ">" ): return Operators::GT;
        case _hash( "<=" ): return Operators::LEq;
        case _hash( ">=" ): return Operators::GEq;
        case _hash( "==" ): return Operators::EqualEq;
        case _hash( "!=" ): return Operators::NEqual;
        case _hash( "&&" ): return Operators::And;
        case _hash( "||" ): return Operators::Or;
        case _hash( "!" ): return Operators::Not;
        default: return Operators::Invalid_Op;
    }
}

Delimiters Delimiter_FromString( const std::string &str )
{
    switch( _hash( str.c_str() ) )
    {
        case _hash( ";" ): return Delimiters::Semi;
        case _hash( "=" ): return Delimiters::Equal;
        case _hash( "(" ): return Delimiters::LParen;
        case _hash( ")" ): return Delimiters::RParen;
        case _hash( "{" ): return Delimiters::LBrace;
        case _hash( "}" ): return Delimiters::RBrace;
        case _hash( "[" ): return Delimiters::LSquare;
        case _hash( "]" ): return Delimiters::RSquare;
        case _hash( "," ): return Delimiters::Comma;
        case _hash( "." ): return Delimiters::Period;
        default:           return Delimiters::Invalid_Delim;
    }
}

bool is_alpha( char c ){ return ( c <= 'z' && c >= 'a' ) || ( c <= 'Z' && c >= 'A' ); }
bool is_digit( char c ){ return ( c <= '9' && c >= '0' ); }

Identifiers Identifier_FromString( const string &str )
{
    if( !is_alpha( str[0] ) )
        return Identifiers::Invalid_Identifier;

    for( char c : str )
        if( !is_alpha( c ) && !is_digit( c ) )
            return Identifiers::Invalid_Identifier;

    return Identifiers::Valid_Identifier;
}

Numbers Number_FromString( const string &str )
{
    if( str[0] == '0' && str.length() > 1 )
        return Numbers::Invalid_Number;

    for( char c : str )
        if( !is_digit( c ) )
            return Numbers::Invalid_Number;

    return Numbers::Valid_Number;
}

const string RWord_ToString( ReservedWords r )
{
    switch( r )
    {
		case ReservedWords::Class: return  std::string( "class" );
		case ReservedWords::Public: return std::string( "public" );
		case ReservedWords::Static: return std::string( "static" );
		case ReservedWords::Extends: return std::string( "extends" );
		case ReservedWords::Void: return   std::string( "void" );
		case ReservedWords::Int: return    std::string( "int" );
		case ReservedWords::Bool: return   std::string( "boolean" );
		case ReservedWords::If: return     std::string( "if" );
		case ReservedWords::Else: return   std::string( "else" );
		case ReservedWords::While: return  std::string( "while" );
		case ReservedWords::Return: return std::string( "return" );
		case ReservedWords::Null: return   std::string( "null" );
		case ReservedWords::True: return   std::string( "true" );
		case ReservedWords::False: return  std::string( "false" );
		case ReservedWords::This: return   std::string( "this" );
		case ReservedWords::New: return    std::string( "new" );
		case ReservedWords::String: return std::string( "String" );
		case ReservedWords::Main: return   std::string( "main" );
		case ReservedWords::Println: return std::string( "System.out.println" );
        default: return std::string( "Unknown RWord." );
    }
}

const string Operator_ToString( Operators o )
{
    switch( o )
    {
        case Operators::Plus:    return std::string( "+" ); 
        case Operators::Minus:   return std::string( "-" ); 
		case Operators::Mult:    return std::string( "*" ); 
		case Operators::Div:     return std::string( "/" ); 
		case Operators::LT:      return std::string( "<" ); 
		case Operators::GT:      return std::string( ">" ); 
		case Operators::LEq:     return std::string( "<=" );
		case Operators::GEq:     return std::string( ">=" );
		case Operators::EqualEq: return std::string( "==" );
		case Operators::NEqual:  return std::string( "!=" );
		case Operators::And:     return std::string( "&&" );
		case Operators::Or:      return std::string( "||" );
		case Operators::Not:     return std::string( "!" );
        default:                 return std::string( "Unknown Operator" );
    }
}

const string Delimiter_ToString( Delimiters d )
{
    switch( d )
    {
        case Delimiters::Semi:    return std::string( ";" );
        case Delimiters::Equal:   return std::string( "=" );
        case Delimiters::LParen:  return std::string( "(" );
        case Delimiters::RParen:  return std::string( ")" );
        case Delimiters::LBrace:  return std::string( "{" );
        case Delimiters::RBrace:  return std::string( "}" );
        case Delimiters::LSquare: return std::string( "[" );
        case Delimiters::RSquare: return std::string( "]" );
        case Delimiters::Comma:   return std::string( "," );
        case Delimiters::Period:  return std::string( "." );
        case Delimiters::Invalid_Delim: 
        default:
                                  return std::string( "Unknown Delimiter" );
    }
}

} // End Namespace <Utils>
