#ifndef SIMPLE_TOKEN_H
#define SIMPLE_TOKEN_H 1

#include <string>
#include <memory>

namespace lex {

constexpr unsigned int _hash(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (_hash(str, h+1)*33) ^ str[h];
}

class Token
{
protected:
    int32_t linenumber;
    int32_t position;
    
    Token( int32_t ln, int32_t pos ) : linenumber( ln ), position( pos ) {}

public: 
    virtual const std::string format() const = 0;

    int32_t ln()  const { return linenumber; }
    int32_t pos() const { return position; }
};

class ReservedWord : public Token
{
    ReservedWord();

public:

    typedef enum 
    {
        Invalid_RWord = 0,
        Class,  Public, Static, 
        Extends,Void,   Int, 
        Bool,   If,     Else, 
        While,  Return, Null, 
        True,   False,  This, 
        New,    String, Main, 
        Println
    } RWord;

    ReservedWord( RWord, int32_t ln, int32_t pos );
    
    static RWord from_string( const std::string & );
    static const std::string to_string( RWord );
    const std::string format() const;

private:
    RWord m_type;
};

class Operator : public Token
{
    Operator();

public:

    typedef enum 
    {
        Invalid_Op = 0,
        Plus,   Minus,  Mult,
        Div,    LT,     GT,
        LEq,    GEq,    EqualEq,
        NEqual, And,    Or,
        Not
    } Op;

    Operator( Op, int32_t ln, int32_t pos );
    
    static Op from_string( const std::string & );
    static const std::string to_string( Op );
    const std::string format() const;

private:
    Op m_type;
};

class Delimiter : public Token
{
    Delimiter();

public:

    typedef enum 
    {
        Invalid_Delim = 0,
        Semi,    Equal,   LParen,
        RParen,  LBrace,  RBrace,
        LSquare, RSquare, Comma,
        Period
    } Delim;

    Delimiter( Delim, int32_t ln, int32_t pos );
    
    static Delim from_string( const std::string & );
    static const std::string to_string( Delim );
    const std::string format() const;

private:
    Delim m_type;
};

class Identifier : public Token
{
    Identifier();

public:
    Identifier( std::string, int32_t ln, int32_t pos );

    const std::string format() const;

private:
    std::string m_id;
};

class Number : public Token
{
    Number();

public:
    Number( int32_t, int32_t ln, int32_t pos );

    const std::string format() const;

private:
    int32_t m_value;
};

/**
 *  Class for representing an error.
 */
class Error : public Token
{
    Error();
public:
    /**
     *  args:
     *  ProblemChars
     *  LineNumber
     *  ErrorMessage
     */
    Error( std::string input,
          uint32_t ln,
          uint32_t pos,
          std::string error);

    const std::string errmsg() const;
    const std::string format() const;
    
private:
    uint32_t    m_linenumber;
    std::string m_error_line;
    std::string m_errmsg;

};


/**
 *  Token Factory.
 */
class TokenFactory
{
    TokenFactory(){}

public:
    static std::shared_ptr<Token> FromString( const std::string & );
};

} // End Namespace <lex>

#endif // SIMPLE_TOKEN_H
