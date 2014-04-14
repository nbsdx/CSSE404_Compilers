
#ifndef CONTEXT_H
#define CONTEXT_H 1

#include <map>
#include <stack>
#include <string>

using namespace std;

class Entry
{
    Entry *m_prev,
          *m_next;

    Entry *c_prev;

    string m_name;
    string m_type;

public:
    Entry( const string &name, const string &type );
    Entry();
    ~Entry();

    inline void set_next( Entry *e ){ this->m_next = e; }
    inline void set_prev( Entry *e ){ this->m_prev = e; }
    inline void append( Entry *e ){
        if( m_next == nullptr )
        {
            m_next = e;
            e->m_prev = this;
        }
        else
        {
            Entry *e2;
            for( e2 = m_next; e2->m_next->m_next != nullptr; e2 = e2->m_next );
            e2->m_next = e;
            e->m_prev = e2;
        }
    }

    Entry *next(){ return m_next; }
    Entry *prev(){ return m_prev; }
    
    string name(){ return m_name; }
    string type(){ return m_type; }

    inline void set_prev_chain( Entry *e ){ this->c_prev = e; }
};

class Context
{
    map< string, Entry* > entries;
    stack< Entry *> handles;
    
public:
    void enter();
    void leave();

    string typeof( const string &name );
    bool defined( const string &name );

    void add( const string &name, const string &type );
};

#endif
