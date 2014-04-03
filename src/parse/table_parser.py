
import json

class Production:
    def __init__( self ):
        self.rhs = []
        self.dest = {}

    def setLHS( self, name ):
        self.name = name;

    def pushSymbol( self, symbol ):
        self.rhs.append( symbol );

    def toJson( self ):
        return { self.name : self.rhs }

    def __repr__( self ):
        return json.dumps( { self.name : self.rhs } )

    def __str__( self ):
        return json.dumps( { self.name : self.rhs } )

class ProdGroup:
    def __init__( self, name ):
        self.name = name
        self.prods = []
        self.dest = {}

    def add( self, prod ):
        self.prods.append( prod )

    def setDest( self, terminal, dest_prod ):
        self.dest[ terminal ] = dest_prod

table = json.loads( open( 'parse_table.json' ).read() )
rulelines = open( '../../doc/rawgrammar.txt' ).read().split( '\n' )

rules = []
groups = []

# Get our productions.
for line in rulelines:

    if not line:
        continue

    tokens = [ x.strip() for x in line.strip().split( ' ' ) ]

    p = Production()

    p.setLHS( tokens[ 0 ] )

    if tokens[0] not in [g.name for g in groups]:
        groups.append( ProdGroup( tokens[0] ) )
#        print "Added Group: " + groups[-1].name

    for token in tokens[ 2: ]:

        if not token:
            continue

        p.pushSymbol( token )

    groups[-1].add( p )
    rules.append( p )

# Make our table into something usable.
i_row = 0
for row in table[2:]:

    # Get useful numbers.
    nrow = [-1 if x==69 or x==70 else x-1 for x in row][1:]

#    print "PARSING ROW FOR PROD: [" + groups[i_row].name + "]"

    # First column is trash.
    col = 1
    for next_prod in nrow:
        if next_prod == -1:
            col = col + 1
            continue

        groups[ i_row ].setDest( table[0][col], next_prod )
#        print "\tAdded " + str(table[0][col]) + " -> " + str(next_prod) + " to " + groups[ i_row ].name

        col = col + 1

    i_row = i_row + 1

# At this point we have the table encoded, and
# the relations all setup. Now to generate the
# C++ file...

file_template = """
/**
    AUTO GENERATED FILE DO NOT MODIFY
**/

#ifndef TRANSITION_TABLE_H
#define TRANSITION_TABLE_H 1

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <memory>

#include "../lex/token.h"

using namespace std;
using namespace lex;

struct Production
{
    string
            lhs;
    shared_ptr<vector<shared_ptr<Token>>>
            rhs;

    Production( string lhs,
                shared_ptr<vector<shared_ptr<Token>>> rhs )
    {
        this->lhs = lhs;
        this->rhs = rhs;
    }

    Production(){}
};

class TransitionTable
{
    vector< Production > productions;
    vector< map< string, int > > table;

    int indexOf( const vector<string> &vec, const string& value ) {
        vector<string>::const_iterator it;
        int index;

        for( index = 0, it = vec.begin(); it != vec.end(); ++it, ++index )
        {
            if( value.compare( *it ) == 0 )
                return index;
        }

        return -1;
    }

public:
    TransitionTable()
    {
        shared_ptr<vector<shared_ptr<Token>>> tvec;
        map<string, int> tmap;
        // Production creation goes here.
%s

        // Table creation goes here.
%s
    }

    const Production &getProduction( int i ){
        return productions.at( i );
    }

    /**
        Return:
            >= 0 : Index of the next Production's ID into the current Production
            -1   : The non-terminal is not refrenced from the current Production
                   It may be ID, Integer, or an Error.

            TODO: Fix this.
    */
    int getNextProdIndex( int curProd, const string &nonTerminal ) {
        if( table[curProd].count( nonTerminal ) == 0 )
            return -1;
        else
            return table[curProd][nonTerminal];
    }
};

#endif

"""

# Build the operations to fill the productions variable.
productions = ""
index = 0

for g in groups:
    for p in g.prods:
        productions += ( "\t// Production: %d\n" % index )
        productions += ( "\t\ttvec = shared_ptr<vector<shared_ptr<Token>>>( new vector<shared_ptr<Token>>() );\n" )
        for t in p.rhs:
            productions += ( "\t\ttvec->push_back( TokenFactory::FromString( \"%s\" ) );\n" % t )
        productions += ( "\t\tproductions.push_back( Production( \"%s\", tvec ) );\n" % g.name )
        index = index + 1

# Build the actual transition table.
table = ""
index = 0
index_ = 0

for g in groups:
    table += ( "\t// Table: %d\n" % index_ )
    for tok in g.dest:
        table += ( "\t\ttmap[\"%s\"] = %d;\n" % ( tok, g.dest[tok] ) )
    for p in g.prods:
        table += ( "\t\ttable.push_back( tmap );\n" )
        index = index + 1
    table += "\t\ttmap.clear();\n"
    index_ = index_ + 1

outfile = open( "tt.hpp", "w+" )
outfile.write( ( file_template % (productions, table) ) )
outfile.close()

# Debug - output rules.
#for g in groups:
#    print "Production [" + g.name + "]:"
#
#    for p in g.prods:
#        print "\t" + str(p.rhs)
#
#    print "Look-A-Heads:"
#
#    for d in g.dest:
#        print "\t" + str(d) + "\t" + str(g.dest[d])
#
#    print ""

