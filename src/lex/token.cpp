#include "token.h"
#include <iostream>

namespace lex {

/**
 *
 *  Token Class Functions.
 *
 */
ReservedWord::ReservedWord( RWord r )
{
    this->m_type = r;

//    std::cout << format() << std::endl;
}

ReservedWord::RWord ReservedWord::from_string( const std::string &str )
{
    switch( _hash( str.c_str() ) )
    {
		case _hash( "class" ):  return ReservedWord::Class;
		case _hash( "public" ): return ReservedWord::Public;
		case _hash( "static" ): return ReservedWord::Static;
		case _hash( "extends" ):return ReservedWord::Extends;
		case _hash( "void" ):   return ReservedWord::Void;
		case _hash( "int" ):    return ReservedWord::Int;
		case _hash( "boolean" ):return ReservedWord::Bool;
		case _hash( "if" ):     return ReservedWord::If;
		case _hash( "else" ):   return ReservedWord::Else;
		case _hash( "while" ):  return ReservedWord::While;
		case _hash( "return" ): return ReservedWord::Return;
		case _hash( "null" ):   return ReservedWord::Null;
		case _hash( "true" ):   return ReservedWord::True;
		case _hash( "false" ):  return ReservedWord::False;
		case _hash( "this" ):   return ReservedWord::This;
		case _hash( "new" ):    return ReservedWord::New;
		case _hash( "String" ): return ReservedWord::String;
		case _hash( "main" ):   return ReservedWord::Main;
		case _hash( "System.out.println" ): return ReservedWord::Println;
        default: return ReservedWord::Invalid_RWord;
    }
}

const std::string ReservedWord::to_string( ReservedWord::RWord r )
{
    switch( r )
    {
		case ReservedWord::Class: return  std::string( "class" );
		case ReservedWord::Public: return std::string( "public" );
		case ReservedWord::Static: return std::string( "static" );
		case ReservedWord::Extends: return std::string( "extends" );
		case ReservedWord::Void: return   std::string( "void" );
		case ReservedWord::Int: return    std::string( "int" );
		case ReservedWord::Bool: return   std::string( "boolean" );
		case ReservedWord::If: return     std::string( "if" );
		case ReservedWord::Else: return   std::string( "else" );
		case ReservedWord::While: return  std::string( "while" );
		case ReservedWord::Return: return std::string( "return" );
		case ReservedWord::Null: return   std::string( "null" );
		case ReservedWord::True: return   std::string( "true" );
		case ReservedWord::False: return  std::string( "false" );
		case ReservedWord::This: return   std::string( "this" );
		case ReservedWord::New: return    std::string( "new" );
		case ReservedWord::String: return std::string( "String" );
		case ReservedWord::Main: return   std::string( "main" );
		case ReservedWord::Println: return std::string( "System.out.println" );
        default: return std::string( "Unknown RWord." );
    }
}

const std::string ReservedWord::format() const
{
    return std::string( "ReservedWord, " ).append( to_string( this->m_type ) );
}

/**
 *  Operator Class functions
 */
Operator::Operator( Op o )
{
    this->m_type = o;
//    std::cout << format() << std::endl;
}

Operator::Op Operator::from_string( const std::string &str )
{
    switch( _hash( str.c_str() ) )
    {
        case _hash( "+" ): return Operator::Plus;
        case _hash( "-" ): return Operator::Minus;
        case _hash( "*" ): return Operator::Mult;
        case _hash( "/" ): return Operator::Div;
        case _hash( "<" ): return Operator::LT;
        case _hash( ">" ): return Operator::GT;
        case _hash( "<=" ): return Operator::LEq;
        case _hash( ">=" ): return Operator::GEq;
        case _hash( "==" ): return Operator::EqualEq;
        case _hash( "!=" ): return Operator::NEqual;
        case _hash( "&&" ): return Operator::And;
        case _hash( "||" ): return Operator::Or;
        case _hash( "!" ): return Operator::Not;
        default: return Operator::Invalid_Op;
    }
}

const std::string Operator::to_string( Operator::Op o )
{
    switch( o )
    {
        case Operator::Plus:    return std::string( "+" ); 
        case Operator::Minus:   return std::string( "-" ); 
		case Operator::Mult:    return std::string( "*" ); 
		case Operator::Div:     return std::string( "/" ); 
		case Operator::LT:      return std::string( "<" ); 
		case Operator::GT:      return std::string( ">" ); 
		case Operator::LEq:     return std::string( "<=" );
		case Operator::GEq:     return std::string( ">=" );
		case Operator::EqualEq: return std::string( "==" );
		case Operator::NEqual:  return std::string( "!=" );
		case Operator::And:     return std::string( "&&" );
		case Operator::Or:      return std::string( "||" );
		case Operator::Not:     return std::string( "!" );
        default:                return std::string( "Unknown Operator" );
    }
}

const std::string Operator::format() const
{
    return std::string( "Operator, " ).append( to_string( this->m_type ) );
}

/**
 *  Delimiter Class Functions
 */
Delimiter::Delimiter( Delim d )
{
    this->m_type = d;
//    std::cout << format() << std::endl;
}

Delimiter::Delim Delimiter::from_string( const std::string &str )
{
    switch( _hash( str.c_str() ) )
    {
        case _hash( ";" ): return Delimiter::Semi;
        case _hash( "=" ): return Delimiter::Equal;
        case _hash( "(" ): return Delimiter::LParen;
        case _hash( ")" ): return Delimiter::RParen;
        case _hash( "{" ): return Delimiter::LBrace;
        case _hash( "}" ): return Delimiter::RBrace;
        case _hash( "[" ): return Delimiter::LSquare;
        case _hash( "]" ): return Delimiter::RSquare;
        case _hash( "," ): return Delimiter::Comma;
        case _hash( "." ): return Delimiter::Period;
        default:           return Delimiter::Invalid_Delim;
    }
}

const std::string Delimiter::to_string( Delimiter::Delim d )
{
    switch( d )
    {
        case Delimiter::Semi:    return std::string( ";" );
        case Delimiter::Equal:   return std::string( "=" );
        case Delimiter::LParen:  return std::string( "(" );
        case Delimiter::RParen:  return std::string( ")" );
        case Delimiter::LBrace:  return std::string( "{" );
        case Delimiter::RBrace:  return std::string( "}" );
        case Delimiter::LSquare: return std::string( "[" );
        case Delimiter::RSquare: return std::string( "]" );
        case Delimiter::Comma:   return std::string( "," );
        case Delimiter::Period:  return std::string( "." );
        case Delimiter::Invalid_Delim: return std::string( "Unknown Delimiter" );
    }
}

const std::string Delimiter::format() const
{
    return std::string( "Delimiter, " ).append( to_string( this->m_type ) );
}

/**
 *  Identifier Class Functions
 */
Identifier::Identifier( std::string s )
{
    this->m_id = s;
//    std::cout << format() << std::endl;
}

const std::string Identifier::format() const
{
    return std::string( "ID, " ).append( this->m_id );
}

/**
 *  Number Class Functions
 */
Number::Number( int32_t value )
{
    this->m_value = value;
//    std::cout << format() << std::endl;
}

const std::string Number::format() const
{
    return std::string( "Integer, " ).append( std::to_string( this->m_value ) );
}

} // End Namespace <lex>
