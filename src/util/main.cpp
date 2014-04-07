
#include <iostream>
#include <string>
#include <typeinfo>
#include "token.h"

template<typename T>
void print( T t )
{
    if( typeid( T ) == typeid( int ) )
        std::cout << "YOU WIN!\n";
    else
        std::cout << "YOU SUCK!!\n";
}

template <>
void print<int>( int t )
{
    std::cout << "Does this work?\n";
}

typedef enum {
    A, B, C
} ID;

int main( void )
{
    Delimiter *d = new Delimiter( 0, 0, Delimiters::LBrace );
    ReservedWord *r1 = new ReservedWord( 0, 0, ReservedWords::New );
    Identifier *id = new Identifier( 0, 0, Identifiers::Valid_Identifier, "var1" );
    Number *num = new Number( 0, 0, Numbers::Valid_Number, 23 );
    ID a = ID::A;
        

    std::cout << "Delimiter: " << d->to_string() << std::endl;
    std::cout << "RWord:     " << r1->to_string() << std::endl;
    std::cout << "Ident:     " << id->to_string() << std::endl;
    std::cout << "Number:    " << num->to_string() << std::endl;

    print( "test" );
    print( std::string( "test2" ) );
    print( 3 );

    if( typeid( a ) == typeid( ID ) )
        std::cout << "True: " << a << "\n";

    return 0;
}
