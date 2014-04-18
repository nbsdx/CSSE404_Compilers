#include "context.h"

#include <string>
#include <iostream>

using namespace std;

Entry::Entry( const string &name, const string &type )
{
    this->m_name = string( name );
    this->m_type = string( type );
    this->m_prev = nullptr;
    this->m_next = nullptr;
    this->c_prev = nullptr;
}

Entry::Entry()
{
    this->m_name = "";
    this->m_type = "";
    this->m_prev = nullptr;
    this->m_next = nullptr;
    this->c_prev = nullptr;
}

Entry::~Entry()
{
    // prev <--> this <--> next
    // prev <--> next
    
    // Both non-null
    if( this->m_prev && this->m_next )
    {
        this->m_next->m_prev = this->m_prev;
        this->m_prev->m_next = this->m_next;
    }
    // m_next is null
    else if( this->m_prev )
    {
        this->m_prev->m_next = nullptr;
    }
    // m_prev is null
    else if( this->m_next )
    {
        this->m_next->m_prev = nullptr;
    }
    // both are null (should never happen)
    else ; // Do nothing.

//    cout << "Deleting: " << m_name << " " << m_type << endl;

    if( c_prev )
        delete c_prev;
}

void Entry::append( Entry *e )
{
    // This is the last entry in the list.
    if( this->m_next == nullptr )
    {
        this->m_next = e;
        e->m_prev = this;
    }
    // This is not the last entry. 
    else
    {
        e->m_next = this->m_next;
        e->m_prev = this;
        this->m_next->m_prev = e;
        this->m_next = e;
    }
}

Context::Context( bool persistant )
{
    this->m_persist = persistant;
}

void Context::enter()
{
    handles.push( new Entry() );
}

void Context::leave()
{
    Entry *e = handles.top();
    handles.pop();
    delete e;
}

bool Context::defined( const string &name )
{
    auto x = entries.find( name );
    if( x != entries.end() )
    {
        if( x->second->next() == nullptr )
            return false;
        else 
            return true;
    }

    return false;
}

bool Context::defined( const string &ns, const string &name )
{
    return ns_defined( ns ) && refrences[ns]->defined( name );
}

bool Context::ns_defined( const string &ns )
{
    return refrences.find( ns ) != refrences.end();
}

string Context::typeof( const string &name )
{
    if( defined( name ) )
        return entries[name]->next()->type();
    return "Undefined";
}

string Context::typeof( const string &ns, const string &name )
{
    if( ns_defined( ns ) )
        return refrences[ns]->typeof( name );
    return "Undefined";
}

void Context::add( const string &name, const string &type )
{
    Entry *e = new Entry( name, type );

    e->set_prev_chain( handles.top() );
    handles.pop();
    handles.push( e );

    if( entries.count( name ) == 0 )
    {
        entries[ name ] = new Entry(); // Placeholder for linked list.
    }

    entries[ name ]->append( e );
}

void Context::add( const string &name, Context *context )
{
    if( refrences.find( name ) == refrences.end() )
        refrences.insert( pair<string, Context*>( name, context ) );
    else
        cerr << "Namespace [" << name << "] already defined" << endl;
}

void Context::add( const string &ns, const string &name, const string &type )
{
    if( refrences.find( ns ) != refrences.end() )
        refrences[ns]->add( name, type );
    else
        cerr << "Namespace [" << ns << "] does not exist" << endl;
}

void Context::setType( const string &name, const string &type )
{
    if( defined( name ) )
        entries[name]->next()->setType( type );
    else
        cout << "Object " << name << " is not defined." << endl;
}

void Context::setType( const string &ns, const string &name, const string &type )
{
    if( refrences.find( ns ) != refrences.end() )
        refrences[ns]->setType( name, type );
    else
        cerr << "Namespace [" << ns << "] does not exist" << endl;
}

Context *Context::getNamespace( const string &ns )
{
    if( refrences.find( ns ) != refrences.end() )
        return refrences[ns];
    return nullptr;
}

/**
 *  Add another context into this context.
 *  If the other context should persist, copy
 *  instead of direct insertion.
 */
void Context::merge( Context *c )
{
    for( auto i : c->entries )
    {
        // Disallow shadowing
        if( this->defined( i.first ) )
        {
            cout << "Error: [" << i.first << "] already defined." << endl;
        }
        else
        {
            Entry *e = i.second->next();

            if( c->m_persist )
            {
                e = new Entry( e->name(), e->type() );
            }

            e->set_prev_chain( handles.top() );
            handles.pop();
            handles.push( e );

            if( entries.count( i.first ) == 0 )
                entries[ i.first ] = new Entry();

            entries[ i.first ]->append( e );
        }
    }
}

void Context::print ( int depth ) {

    cout << "Visable entries:" << endl;
    for( auto i : this->entries ) {
        if( i.second->next() )
            cout << "K: " << i.first << " V: " << typeof( i.first ) << "\n";
    }

    if( depth > 0 )
    {
        cout << "There are " << refrences.size() << " namespaces defined" << endl;
        for( auto i : this->refrences ) {
            cout << "Name: " << i.first << endl;
            i.second->print( depth - 1);
        }
    }
}
