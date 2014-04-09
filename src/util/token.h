#ifndef SIMPLE_TOKEN_H
#define SIMPLE_TOKEN_H 1

#include <string>
#include <iostream>
#include <typeinfo>

using namespace std;

constexpr unsigned int _hash(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (_hash(str, h+1)*33) ^ str[h];
}

typedef enum ReservedWords_ {
    Invalid_RWord = 0,
    Class,  Public, Static, 
    Extends,Void,   Int, 
    Bool,   If,     Else, 
    While,  Return, Null, 
    True,   False,  This, 
    New,    String, Main, 
    Println
} ReservedWords;

typedef enum Operators_ {
    Invalid_Op = 0,
    Plus,   Minus,  Mult,
    Div,    LT,     GT,
    LEq,    GEq,    EqualEq,
    NEqual, And,    Or,
    Not
} Operators;

typedef enum Delimiters_ { 
    Invalid_Delim = 0,
    Semi,    Equal,   LParen,
    RParen,  LBrace,  RBrace,
    LSquare, RSquare, Comma,
    Period
} Delimiters;

typedef enum Identifiers_ {
    Invalid_Identifier = 0,
    Valid_Identifier
} Identifiers;

typedef enum Numbers_ {
    Invalid_Number = 0,
    Valid_Number
} Numbers;

// Mark this shit as exterm
namespace Util {


extern ReservedWords RWord_FromString( const std::string &str );

extern Operators Operator_FromString( const std::string &str );

extern Delimiters Delimiter_FromString( const std::string &str );

extern bool is_alpha( char c );

extern bool is_digit( char c );

extern Identifiers Identifier_FromString( const string &str );

extern Numbers Number_FromString( const string &str );

extern const string RWord_ToString( ReservedWords r );

extern const string Operator_ToString( Operators o );

extern const string Delimiter_ToString( Delimiters d );

} // End Namespace <Utils>

class BasicToken
{
public:
    BasicToken( ) {}
    BasicToken( const string &str ) : m_raw( str ) {}
    virtual ~BasicToken(){}

    bool operator==( const BasicToken &other ) {
        return ( other.m_raw.compare( m_raw ) == 0 );
    }
    bool operator!=( const BasicToken &other ) {
        return !( *this == other );
    }

    string raw() {
        return m_raw;
    }

protected:

    std::string m_raw;
};

struct NonTerminal : BasicToken
{
    NonTerminal( const string &str ) : BasicToken( str ){}
    ~NonTerminal() {}
};

struct Epsilon : BasicToken
{
    Epsilon() : BasicToken( "" ) {}
    ~Epsilon() {}
};

struct Separator : BasicToken
{
    Separator() : BasicToken ( "" ) {}
    ~Separator() {}
};

struct EndOfStack : BasicToken
{
    EndOfStack() : BasicToken( "$$" ) {}
    ~EndOfStack() {}
};

struct ErrorToken : BasicToken
{
    ErrorToken() : BasicToken( "ERROR" ) {}
    ~ErrorToken() {}
};


template
<typename TokenType, typename ValueType>
class LexerToken : public BasicToken
{
public: 
    LexerToken( int32_t line,
                int32_t pos,
                TokenType token,
                ValueType value ) : 
        BasicToken( to_string( token, value ) )
    {
        this->m_line = line;
        this->m_pos = pos;
        this->m_token = token;
        this->m_value = value;
    }

    LexerToken( int32_t line,
                int32_t pos,
                TokenType token ) : 
        BasicToken( to_string( token ) )
    {
        this->m_line = line;
        this->m_pos = pos;
        this->m_token = token;
        if( typeid( ValueType ) == typeid( string ) )
            this->m_value = this->raw();
        else
            this->m_value = ValueType();
    }

    ~LexerToken()
    {
        // Do nothing.
    }

    const string to_string()
    {
        try {
            return to_string( m_token );
        } catch( char const *s ) {
            try {
                return to_string( m_token, m_value );
            } catch( char const *s )
            {
                std::cout << "Uhh... " << s << std::endl;
            }
        }
    }

    const string to_string_( Numbers token, void *value ) const
    {
        if( Util::Number_FromString( std::to_string( *(int32_t*) value ) ) ==
                Numbers::Valid_Number )
            return std::to_string( *(int32_t*) value );
        else
        {
//            std::cout << "\nInvliad Number Token: " << *(int32_t*) value << std::endl; 
            return "Invalid Number Token";
        }
    }

    const string to_string_( Identifiers token, void *value ) const
    {
        if( Util::Identifier_FromString( *(const string*)value ) ==
                Identifiers::Valid_Identifier )
            return *(const string*) value;
        else
            return "Invalid Identifier Token";
    }

    // TODO:
    template <typename T, typename V>
    const string to_string( T token, V value ) const 
    {
//        cout << "DEBUG: " << value << endl;

        if( typeid( T ) == typeid( Identifiers ) )
        {
            return to_string_( (Identifiers) token, (void*)&value );
        }
        else if( typeid( T ) == typeid( Numbers ) )
        {
            return to_string_( (Numbers) token, (void*)&value );
        }
        else
        {
//            std::cout << "To String <T,V>\n";
            throw "Unsupported Type";
        }
    }
    
    const string to_string( TokenType token ) const
    {
        if( typeid( TokenType ) == typeid( ReservedWords ) )
            return Util::RWord_ToString( ( ReservedWords ) token );
        else if( typeid( TokenType ) == typeid( Operators ) )
            return Util::Operator_ToString( ( Operators ) token );
        else if( typeid( TokenType ) == typeid( Delimiters ) )
            return Util::Delimiter_ToString( ( Delimiters ) token );
        else
        {
//            std::cout << "To String (token)\n";
            throw "Unsupported Type";
        }
    }

    template <typename T>
    T from_string( const string &s ) const
    {
        if( typeid( T ) == typeid( ReservedWords ) )
            return Util::RWord_FromString( s );
        else if( typeid( T ) == typeid( Operators ) )
            return Util::Operator_FromString( s );
        else if( typeid( T ) == typeid( Delimiters ) )
            return Util::Delimiter_FromString( s );
        else if( typeid( T ) == typeid( Identifiers ) )
            return Util::Identifier_FromString( s );
        else if( typeid( T ) == typeid( Numbers ) )
            return Util::Number_FromString( s );
        else
            return "Unsupported Type";
    }

    inline int32_t line()  const { 
        return m_line; 
    }
    inline int32_t pos() const { 
        return m_pos; 
    }
    inline const ValueType &value() const { 
        return m_value; 
    }
    inline TokenType token() const { 
        return m_token; 
    }

    bool operator==( const LexerToken<TokenType,ValueType> &other ) {
        return this->m_token == other.m_token;
    }

//    bool operator==( const BasicToken &other ) {
//        return this->m_raw.compare( other.raw() ) == 0;
//    }

    bool operator!=( const LexerToken<TokenType,ValueType> &other ) {
        return !( *this == other );
    }

//    bool operator!=( const BasicToken &other ) {
//        return !( *this == other );
//    }

private:
    int32_t m_line;
    int32_t m_pos;
    TokenType m_token;
    ValueType m_value;
};

typedef LexerToken<ReservedWords, string> ReservedWord;
typedef LexerToken<Identifiers, string> Identifier;
typedef LexerToken<Delimiters, string> Delimiter;
typedef LexerToken<Operators, string> Operator;
typedef LexerToken<Numbers, int32_t> Number;

struct EndOfFileToken : BasicToken
{
    EndOfFileToken() : BasicToken( "$" ) {}
    ~EndOfFileToken() {}
};

class LexerTokenFactory
{
public:
    static BasicToken* FromString( const string &str )
    {
        ReservedWords r;
        Delimiters d;
        Operators o;
        Identifiers i;
        Numbers n;

        if( ( r = Util::RWord_FromString( str ) ) != ReservedWords::Invalid_RWord )
            return new ReservedWord( 0, 0, r );
        else if( ( d = Util::Delimiter_FromString( str ) ) != Delimiters::Invalid_Delim )
            return new Delimiter( 0, 0, d );
        else if( ( o = Util::Operator_FromString( str ) ) != Operators::Invalid_Op )
            return new Operator( 0, 0, o );
        else if( ( i = Util::Identifier_FromString( str ) ) != Identifiers::Invalid_Identifier )
            return new Identifier( 0, 0, i, str );
        else if( ( n = Util::Number_FromString( str ) ) != Numbers::Invalid_Number )
            return new Number( 0, 0, n, atoi( str.c_str() ) );
        else
            return nullptr;
    }

};

/**
 *  LexerToken Factory.
 */
/*
class TokenFactory
{
    TokenFactory(){}

public:
    static shared_ptr<LexerToken> FromString( const string & );
};
*/

namespace Util {
    extern void print_token( BasicToken *token );
}

#endif // SIMPLE_TOKEN_H
