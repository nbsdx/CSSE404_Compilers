#include "context.h"

#include <string>
#include <iostream>

using namespace std;

Entry::Entry( const string &name, const string &type )
{
    this->m_name = name;
    this->m_type = type;
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
    if( this->m_prev && this->m_next )
    {
        this->m_next->m_prev = this->m_prev;
        this->m_prev->m_next = this->m_next;
    }
    else if( this->m_prev )
    {
        this->m_prev->m_next = nullptr;
    }
    else if( this->m_next )
    {
        this->m_next->m_prev = nullptr;
    }
    else
        ; // Do nothing.

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
    }

    return true;
}

string Context::typeof( const string &name )
{
    if( defined( name ) )
        return entries[name]->next()->type();
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
            Entry *e = i.second;
            e->set_prev_chain( handles.top() );
            handles.pop();
            handles.push( e );

            if( entries.count( i.first ) == 0 )
                entries[ i.first ] = new Entry();

            entries[ i.first ]->append( e );
        }
    }
}
