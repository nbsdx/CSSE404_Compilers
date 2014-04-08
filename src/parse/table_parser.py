
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
    nrow = [-1 if x==71 or x==72 else x-1 for x in row][1:]

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

#include "../util/token.h"

using namespace std;

struct Production
{
    string
            lhs;
    vector<vector<BasicToken*>>
            rhs;
    int
            g_id;

    Production( string lhs,
                vector<vector<BasicToken*>> rhs,
                int g_id )
    {
        this->lhs = lhs;
        this->rhs = rhs;
        this->g_id = g_id;
    }

    Production(){}
};

class TransitionTable
{
    vector< Production > productions;
    vector< string >     prod_names;
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
        vector<vector<BasicToken*>> tvec;
        map<string, int> tmap;
        // Production creation goes here.
%s

        // Table creation goes here.
%s
    }

    bool is_production( const string &str )
    {
        for( string p : prod_names )
            if( p.compare( str ) == 0 )
                return true;
        return false;
    }

    const Production &getProductionByGroup( int i ){
        return productions.at( i );
    }

    int prodIdToGroupId( int i )
    {
        int index = 0;
        int group_ID = 0;
        for( Production p : productions )
        {
            for( vector<BasicToken*> v : p.rhs )
            {
                if( index == i )
                    return group_ID;
                ++index;
            }
        }
        return -1;
    }

    int prodGroupId( const string &name )
    {
        int group_ID = 0;
        for( Production p : productions )
        {
            if( p.lhs.compare( name ) == 0 )
                return group_ID;
            ++group_ID;
        }
        return -1;
    }

    vector<BasicToken*> getRHSById( int i ){
        int index = 0;
        for( Production p : productions )
        {
            for( vector<BasicToken*> v : p.rhs )
            {
                if( index == i )
                    return v;
                ++index;
            }
        }

        throw "No Such Element";
    }

    /**
        Return:
            >= 0 : Index of the next Production's ID into the current Production
            -1   : The non-terminal is not refrenced from the current Production
                   It may be ID, Integer, or an Error.

            TODO: Fix this.
    */
    int getNextProdIndex( int curProdGroup, const string &nonTerminal ) {
        if( table[curProdGroup].count( nonTerminal ) == 0 )
            return -1;
        else
            return table[curProdGroup][nonTerminal];
    }

private:
    BasicToken* FromString( const string &str )
    {
        // First, check if it's the name of a NonTerminal
        if( str.compare( "EPSILON" ) == 0 )
        {
            return new Epsilon();
        }
        if( str.compare( "ID" ) == 0 )
        {
            return new Identifier( 0, 0, Identifiers::Valid_Identifier, "" );
        }
        if( str.compare( "$" ) == 0 )
        {
            return new EndOfFileToken();
        }
        else if( str.compare( "Integer" ) == 0 )
        {
            return new Number( 0, 0, Numbers::Valid_Number, 0 );
        }
        else if( is_production( str ) )
        {
            return new NonTerminal( str );
        }
        else
        {
            BasicToken *tok = LexerTokenFactory::FromString( str );
            if( tok == nullptr )
                cout << "Unable to parse Token: " << str << endl;
            return tok;
        }
    }
};

#endif

"""



# Remove ' from the production names.
def fix_name( NT ):
    return NT.replace( '\'', '_' )

# Build the operations to fill the productions variable.
productions = ""
index = 0

productions += ("\t\tprod_names.push_back( \"%s\" );\n" * len(groups)) % tuple([fix_name(g.name) for g in groups])

for g in groups:

    productions += ( "\t// Production: %d\n" % index )
    productions += ( "\t\ttvec = {\n" )

    # This generates something like:
    # vector<vector<BasicToken*>> t =
    #   { { PTF::FS( %s ), PTS::FS( %s ) }, ... };

    for p in g.prods:

        productions += ( "\t\t\t{ " )
        productions += ( ( "this->FromString( \"%s\" ), " * len(p.rhs) ) % tuple([fix_name(x) for x in p.rhs]))
        productions += ( " },\n" )

        index = index + 1

    productions += ( "\t\t};\n " )

    productions += ( "\t\tproductions.push_back( Production( \"%s\", tvec, %d ) );\n" % ( fix_name(g.name ), index - 1 ) )
    productions += ( "\t\ttvec.clear();\n" )

# Build the actual transition table.
table = ""
index = 0
index_ = 0

for g in groups:
    table += ( "\t// Table: %d\n" % index_ )
    for tok in g.dest:
        table += ( "\t\ttmap[\"%s\"] = %d;\n" % ( tok, g.dest[tok] ) )

    table += ( "\t\ttable.push_back( tmap );\n" )

    for p in g.prods:
        index = index + 1
    table += "\t\ttmap.clear();\n"
    index_ = index_ + 1

outfile = open( "tt.hpp", "w+" )
outfile.write( ( file_template % (productions, table) ) )
outfile.close()

#print (file_template % (productions, table))

exit()

# Iterate over NTs again, and

# Generate an enum entry for each

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

