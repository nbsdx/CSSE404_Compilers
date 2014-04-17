
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

    inline void set_prev_chain( Entry *e ){ this->c_prev = e; }
    
    /**
     *  Correct way to set the next element in the list.
     */
    void append( Entry *e );

    Entry *next(){ return m_next; }
    Entry *prev(){ return m_prev; }
    
    string name(){ return m_name; }
    string type(){ return m_type; }
};

class Context
{
    map< string, Entry* > entries;
    stack< Entry *> handles;
    
public:
    /**
     *  Begin a new scope (effectively { in c/c++).
     */
    void enter();

    /**
     *  Leave the current scope (effectively } in c/c++).
     */
    void leave();

    string typeof( const string &name );
    bool defined( const string &name );

    /**
     *  Add Varaible "name" to the context with type "type".
     */
    void add( const string &name, const string &type );

    /**
     *  Add a the contents of another context to this context,
     *  at the current scope (effectively calling this->add for
     *  each Entry in c).
     */
    void merge( Context *c );

    /**
     * Print out all entries.
     */
    void print();
};

#endif
