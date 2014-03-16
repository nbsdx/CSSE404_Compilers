#include <cstdlib>
#include <iostream>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "token.cpp"

// Namespaces
namespace msm = boost::msm;
namespace mpl = boost::mpl;

// Globals
std::string token;
std::vector<Token> pgm;

// Convience Macros
#define FSM_State_On_Entry( body ) \
    template <class Event, class FSM> \
    void on_entry( Event const &event, FSM &fsm ) \
        body

#define FSM_State_On_Exit( body ) \
    template <class Event, class FSM> \
    void on_exit( Event const &event, FSM &fsm ) \
        body

#define FSM_State( name, on_enter, on_exit ) \
    struct name : public msm::front::state<> { \
        FSM_State_On_Entry( on_enter ) \
        FSM_State_On_Exit( on_exit ) \
    };

// Events
struct read_plus{};
struct read_minus{};
struct read_number{
public:
    char number;
    read_number( char c ) : number(c){};
};

// Front-end: Define FSM Structure
struct Parser_ : public msm::front::state_machine_def<Parser_>
{
    // FSM States.
    FSM_State( Start, {}, {} );

    FSM_State( ReadPlus,
            {token.clear(); Token tok = Token(Operator, Plus); pgm.push_back(tok);},
            {} );

    FSM_State( ReadMinus,
            {token.clear(); Token tok = Token(Operator, Minus); pgm.push_back(tok);},
            {} );

    struct ReadNumber_ : public msm::front::state_machine_def<ReadNumber_>
    {
        FSM_State_On_Entry( { token.clear(); } );
        FSM_State_On_Exit ( { int32_t val = atoi(token.c_str()); Token tok = Token(Integer, val); 
                              pgm.push_back(tok);} );

        FSM_State( ReadNumber_Internal,
                { token.append( 1, event.number ); },
                {} );

        typedef ReadNumber_Internal initial_state;

        struct transition_table : mpl::vector1<
                                  _row<ReadNumber_Internal, read_number, ReadNumber_Internal > >{};
    };

    typedef msm::back::state_machine<ReadNumber_> ReadNumber;
    typedef Start initial_state;

    // Transition Table for the State Machine.
    struct transition_table : mpl::vector<

        _row< Start,      read_plus, ReadPlus >, _row< Start,      read_minus, ReadMinus >, _row< Start,      read_number, ReadNumber >,
        _row< ReadPlus,   read_plus, ReadPlus >, _row< ReadPlus,   read_minus, ReadMinus >, _row< ReadPlus,   read_number, ReadNumber >,
        _row< ReadMinus,  read_plus, ReadPlus >, _row< ReadMinus,  read_minus, ReadMinus >, _row< ReadMinus,  read_number, ReadNumber >,
        _row< ReadNumber, read_plus, ReadPlus >, _row< ReadNumber, read_minus, ReadMinus >, _row< ReadNumber, read_number, ReadNumber > 
    >{};
};

typedef msm::back::state_machine<Parser_> Parser;

bool is_plus( char c ) { return c == '+'; }
bool is_minus( char c ) { return c == '-'; }
bool is_number( char c ) { return c >= '0' && c <= '9'; }


std::vector<Token> lex (int fd) {
    pgm = std::vector<Token>();

    Parser p;
    p.start();
    char c;

    // Process loop.
    while( read( fd, &c, 1 ) != 0 )
    {
        if( is_plus( c ) )
            p.process_event( read_plus() );
        else if( is_minus( c ) )
            p.process_event( read_minus() );
        else if( is_number( c ) )
            p.process_event( read_number( c ) );
        else if( c == '\n' )
            break;
        else
        {
            std::cout << "Invalid char [" << c << "]. Exiting\n";
            break;
        }
    }

    p.stop();

    return pgm;
}

