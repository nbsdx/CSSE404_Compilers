
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

    void setName( const string &name ){ m_name = name; }
    void setType( const string &type ){ m_type = type; }

    Entry *next(){ return m_next; }
    Entry *prev(){ return m_prev; }
    
    string name(){ return m_name; }
    string type(){ return m_type; }
};

class Context
{
    map< string, Entry* > entries;
    map< string, Context* > refrences;
    stack< Entry *> handles;
    bool m_persist;
    
public:
    
    Context( bool persistant = false );

    /**
     *  Begin a new scope (effectively { in c/c++).
     */
    void enter();

    /**
     *  Leave the current scope (effectively } in c/c++).
     */
    void leave();

    string typeof( const string &name );
    string typeof( const string &ns, const string &name );

    bool defined( const string &name );

    /**
     *  Check for the existence of the given namespace
     */
    bool ns_defined( const string &name );

    /**
     *  Look up the name in the given namespace ns
     */
    bool defined( const string &ns, const string &name );

    /**
     *  Add Varaible "name" to the context with type "type".
     */
    void add( const string &name, const string &type );

    /** 
     *  Add a refrence to another context with the given name.
     */
    void add( const string &name, Context *context );

    /**
     *  Add an entry to a namespace
     */
    void add( const string &ns, const string &name, const string &type );

    void setType( const string &name, const string &type );
    void setType( const string &ns, const string &name, const string &type );

    /**
     *  Add a the contents of another context to this context,
     *  at the current scope (effectively calling this->add for
     *  each Entry in c).
     */
    void merge( Context *c );

    /**
     *  Get the context for a Namespace
     */
    Context *getNamespace( const string& ns );

    /**
     * Print out all entries.
     */
    void print( int depth = 1 );
};

#endif
