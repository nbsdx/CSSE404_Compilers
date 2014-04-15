
#include <iostream>
#include <stack>

#include "context.h"

using namespace std;

/**
Represents a file such as:

    class Test {}
    
    class Test2 {
        int x;
    }

*/

auto main( int argc, char **argv ) -> int
{
    Context *c = new Context();

    cout << "Entering top context." << endl;

    c->enter();

    cout << "Defining [Test] as type [class]" << endl;
    c->add( "Test", "class" );
    
    cout << "Defining [Test2] as type [class]" << endl;
    c->add( "Test2", "class" );

    if( c->defined( "Test" ) )
        cout << "[Test] is defined" << endl;

    cout << "Entering inner context." << endl;
    c->enter();

    cout << "Defining [x] as [int]" << endl;
    c->add( "x", "int" );

    cout << "Type of [x]: " << c->typeof( "x" ) << endl;

    if( c->defined( "Test" ) )
        cout << "[Test] is defined" << endl;
    
    cout << "Leaving inner context." << endl;
    c->leave();

    if( !c->defined( "x" ) )
        cout << "[x] no longer defined." << endl;

    cout << "Leaving top context." << endl;
    c->leave();

    if( !c->defined( "Test2" ) )
        cout << "[Test2] is not defined" << endl;
    
    return 0;
}
