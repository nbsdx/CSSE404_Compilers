
#ifndef LEXER_HPP
#define LEXER_HPP 1

#include <cstdlib>
#include <iostream>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/preprocessor/seq/elem.hpp>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "token.h"

namespace lex {

using namespace std;

// Namespaces
namespace msm = boost::msm;
namespace mpl = boost::mpl;

// Convience Macros
#define UNUSED(expr) do { (void)(expr); } while (0)

#define FSM_State_On_Entry( body ) \
    template <class Event, class FSM> \
    void on_entry( Event const &event, FSM &fsm ) \
        { UNUSED(event); UNUSED(fsm); BOOST_PP_SEQ_ELEM( 0, body ) }

#define FSM_State_On_Exit( body ) \
    template <class Event, class FSM> \
    void on_exit( Event const &event, FSM &fsm ) \
        { UNUSED(event); UNUSED(fsm); BOOST_PP_SEQ_ELEM( 0, body ) }

// DATA is of form (name) (on_entry) (on_exit) 
// Where on_entry/exit are statements.
#define FSM_State( DATA ) \
    struct BOOST_PP_SEQ_ELEM( 0, DATA ) : public msm::front::state<> { \
        FSM_State_On_Entry( BOOST_PP_SEQ_ELEM( 1, DATA ) ) \
        FSM_State_On_Exit(  BOOST_PP_SEQ_ELEM( 2, DATA ) ) \
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
private:
    vector<Token> m_pgm;
    string        m_token;

public:

    vector<Token> &pgm(){ return m_pgm; }
    string &token(){ return m_token; }

    // FSM States.
    FSM_State( (Start) (()) (()) );

    FSM_State( (ReadPlus)
               ((
                 fsm.token().clear(); 
                 Token tok = Token( Token::Operator, Token::Plus );
                 fsm.pgm().push_back( tok );
               ))
               ((
               )) );

    FSM_State( (ReadMinus)
               ((
                 fsm.token().clear(); 
                 Token tok = Token( Token::Operator, Token::Minus );
                 fsm.pgm().push_back(tok);
               ))
               ((
               )) );

    struct ReadNumber_ : public msm::front::state_machine_def<ReadNumber_>
    {
        string m_token;
        public:
        string &token(){ return m_token; }

        FSM_State_On_Entry( (
                    m_token.clear();
                    fsm.token().clear();
                    ) );

        FSM_State_On_Exit ( (
                    int32_t val = atoi( m_token.c_str() ); 
                    Token tok = Token( Token::Integer, val ); 
                    fsm.pgm().push_back( tok );
                    ) );

        FSM_State( (ReadNumber_Internal)
                   ((fsm.token().append( 1, event.number );))
                   (()) );

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

    inline vector<Token> const &get_program(){ return m_pgm; }
};

typedef msm::back::state_machine<Parser_> Parser;

} // End Namespace lex

#endif
