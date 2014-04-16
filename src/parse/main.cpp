
#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include "tt.hpp"
#include "../lex/lexer.h"
#include "AST.h"

using namespace std;



RTree *parse (vector<BasicToken*> pgm);

bool match( BasicToken *t1, BasicToken *t2 )
{
    // Edge case - should never happen.
    if( dynamic_cast<NonTerminal*>( t1 ) && dynamic_cast<NonTerminal*>( t2 ) )
        return *dynamic_cast<NonTerminal*>( t1 ) == *dynamic_cast<NonTerminal*>( t2 );
    
    if( dynamic_cast<ReservedWord*>( t1 ) && dynamic_cast<ReservedWord*>( t2 ) )
        return *dynamic_cast<ReservedWord*>( t1 ) == *dynamic_cast<ReservedWord*>( t2 );
    
    if( dynamic_cast<Delimiter*>( t1 ) && dynamic_cast<Delimiter*>( t2 ) )
        return *dynamic_cast<Delimiter*>( t1 ) == *dynamic_cast<Delimiter*>( t2 );
    
    if( dynamic_cast<Operator*>( t1 ) && dynamic_cast<Operator*>( t2 ) )
        return *dynamic_cast<Operator*>( t1 ) == *dynamic_cast<Operator*>( t2 );
    
    if( dynamic_cast<Number*>( t1 ) && dynamic_cast<Number*>( t2 ) )
        return true;

    if( dynamic_cast<Identifier*>( t1 ) && dynamic_cast<Identifier*>( t2 ) )
        return true;

    return false;
}

// Make Messages more visible:
enum Color {
    None = 0,
    Black, Red, Green, Yellow, Blue, Magenta, Cyan, White
};

string set_color( Color fg = None, Color bg = None )
{
    //char num_s[3];
    string s = "\033[";
    if( !fg && !bg )
        s += "0";

    if( fg )
    {
        s += to_string( 29 + fg );
        if( bg ) 
            s += ";";
    }

    if( bg )
        s += to_string( 39 + bg );

    return s + "m";
};

int main( int argc, char **argv )
{
    if( argc < 2 )
    {
        std::cout << "Error please supply file\n";
        exit( 1 );
    }

    ifstream file( argv[1], ifstream::binary );
    if( !file )
//    if( ( fd = open( argv[1], O_RDONLY ) ) == -1 )
    {
        std::cout << "Error opening file\n";
        exit( 1 );
    }

    // Lexer Output.
    //vector<BasicToken*> pgm = lex::Lexer().lex( fd );
    vector<BasicToken*> pgm = lex::Lexer().lex( file );

    RTree *raw = parse( pgm );

    if( ( argc > 2 ) && ( string( "--print" ).compare( argv[2] ) == 0 ) )
        raw->printT();


    return EXIT_SUCCESS;
}




RTree *parse (vector<BasicToken*> pgm) {

    // Initialize our Transition Table.
    TransitionTable tt;
    vector<BasicToken*>::reverse_iterator rit;
    //int fd;
    bool errored = false;


    //for( BasicToken *token : pgm )
    //{
    //    print_token( token );
    //}

    // Stack of Symbols.
    stack<BasicToken*>  symbols;
    symbols.push( new EndOfStack() );

    // Current Prod ID
    int curProdGroup = 0;

    // Temp storage
    vector<BasicToken*> temp = tt.getRHSById( curProdGroup );

    // AST root
    RTree *root = new RTree( new NonTerminal("Program") );
    
    stack<RTree*> tree;
    tree.push(root);

    symbols.push(new Separator());

    // Push the Start State.
    for( rit = temp.rbegin() ; rit != temp.rend() ; ++rit )
    {
        symbols.push( *rit );
    }

    curProdGroup = 1;

    stack<BasicToken *> recovery;

    recovery.push( new NonTerminal( "Program" ) );
    recovery.push( temp[1] );

    // Loop
    while( symbols.size() > 0 )
    {
        //assert(!tree.empty());

        //cout << "Testing: "; print_token( symbols.top() );
        //cout << "Against: "; print_token( pgm[0] );

        if( dynamic_cast<Epsilon*>( symbols.top() ) )
        {
            symbols.pop();
        }
        else if( dynamic_cast<EndOfFileToken*>( symbols.top() ) )
        {
            //cout << "Finished!" << endl;
            pgm.erase( pgm.begin() );
            symbols.pop();
        }
        else if( dynamic_cast<EndOfStack*>( symbols.top() ) )
        {
            //cout << "Hit bottom of stack." << endl;
            symbols.pop();
        }
        else if ( dynamic_cast<Separator*>( symbols.top() ) )
        {
            symbols.pop();
            //cout << "Merging trees - hit separator\n";
            //cout << "tstack size - " << tree.size() << "\n";
            //cout << "Stack top - " << tree.top()->printVal() << "\n";
            if (tree.size() > 1) {
                RTree *sub = tree.top();
                tree.pop();
                recovery.pop();

                // The below is invariant for non-epsilon branches
                // (We don't want those polluting the AST)
                if (!sub->isLeaf() && sub->degree() > 0)
                    tree.top()->insertSubT( sub );
            }
        }
        else 
recover_label:    
        if( match( symbols.top(), pgm[0] ) )
        {
            // Matched terminal
            RTree *leaf = new RTree(pgm[0]);
            tree.top()->insertSubT( leaf );

            //cout << "MATCH!" << endl;
            symbols.pop();
            pgm.erase( pgm.begin() );

            //tree.pop();
        }
        else
        {
            if( dynamic_cast<NonTerminal*>( symbols.top() ) )
            {
                NonTerminal *nt = dynamic_cast<NonTerminal*>(symbols.top());

                // Add a new tier to the tree - for the prodgroup
                RTree *branch = new RTree( symbols.top() );
                tree.push( branch );

                //cout << "Split NT: "; print_token( nt );
                symbols.pop(); // Dispose of expanded NonTerminal

                recovery.push( symbols.top() );

                // The Separator token is used to trigger a subtree merge
                Separator *sep = new Separator();
                symbols.push( sep );

                int prod_group_id;
                int next_prod_index;

                try
                {
                    prod_group_id = tt.prodGroupId( nt->raw() );

                    // These two are special cases....
                    if( dynamic_cast<Number*>( pgm[0] ) )
                        next_prod_index = tt.getNextProdIndex( prod_group_id, "Integer" );
                    else if( dynamic_cast<Identifier*>( pgm[0] ) )
                        next_prod_index = tt.getNextProdIndex( prod_group_id, "ID" );
                    else
                        next_prod_index = tt.getNextProdIndex( prod_group_id, pgm[0]->raw() );

                    //cout << "NextProdINDX: " << next_prod_index << endl;

                    //cout << "Next Token:   "; print_token( pgm[0] );

                    temp = tt.getRHSById( next_prod_index );
/*
                    cout << "Replacing [" << nt->raw() << "] with: \n[";
                    for( vector<BasicToken*>::iterator it = temp.begin(); it != temp.end(); ++it )
                    {
                        cout << (*it)->raw() << ", ";
                    }
                    cout << "]" <<  endl;
*/
                    for( rit = temp.rbegin(); rit != temp.rend(); ++rit )
                    {
                        symbols.push( *rit );
                    }
                }
                catch( char const *msg )
                {
                    /*
                    cout << "ERROR: " << msg << endl;
                    cout << "Production Group: " << prod_group_id << endl;
                    cout << "Next Production Index: " << next_prod_index << endl;
                    cout << "Using Input: " << pgm[0]->raw() << endl;
                    */
                }
            }
            else
            {
                ReservedWord *rw;
                Operator *op;
                Delimiter *d;
                Identifier *id;
                Number *num;

                errored = true;

                BasicToken *token = pgm[0];
                
                cerr << set_color( Red );
                cerr << "Syntax Error: " << set_color(); 
                if(( rw = dynamic_cast<ReservedWord*>( token ) ))
                    cerr << "Line " << rw->line() << " column: " << rw->pos() << endl;
                else if(( op = dynamic_cast<Operator*>( token ) ))
                    cerr << "Line " << op->line() << " column: " << op->pos() << endl;
                else if(( d = dynamic_cast<Delimiter*>( token ) ))
                    cerr << "Line " << d->line() << " column: " << d->pos() << endl;
                else if(( id = dynamic_cast<Identifier*>( token ) ))
                    cerr << "Line " << id->line() << " column: " << id->pos() << endl;
                else if(( num = dynamic_cast<Number*>( token ) ))
                    cerr << "Line " << num->line() << " column: " << num->pos() << endl;
                
                cerr << set_color() << "Found: " << set_color( Magenta ) << pgm.front()->raw();
                cerr << set_color() << ", expected: " << set_color( Green ) << symbols.top()->raw() << endl;
                cerr << set_color();

                // Try to ignore error.
                // Pop off until the recovery token is found.
                while( symbols.size() > 0 )
                {
                    if( symbols.top() != recovery.top() )
                        symbols.pop();
                    else
                        break;
                }

                if( symbols.size() == 0 )// should never happen
                {
                    cerr << "Unrecoverable Error Occured" << endl;
                    exit( 1 );
                }

                recovery.pop();
                tree.top()->insertSubT( new RTree( new ErrorToken() ) );
                
                if( tree.size() > 1 )
                    tree.pop();

                // Now that the Symbol Stack has been fixed, match the program queue.
                if( dynamic_cast<NonTerminal*>( symbols.top() ) )
                {
                    // Get the first set for the NonTerminal.
                    // Then remove from pgm until the head
                    // matches a first token.
                    vector<string> first = tt.first( symbols.top()->raw() );
                    while( pgm.empty() == false )
                    {
                        for( string s : first )
                        {
                            // If we see one of the first chars for the NT.
                            if( pgm[0]->raw().compare( s ) == 0 )
                            {
                                goto break_early;
                            }
                        }
                        pgm.erase( pgm.begin() );
                    }
break_early:                    
                    if( pgm.empty() )
                    {
                        cerr << "Erron on NT Recovery.\n";
                        cerr << "Unrecoverable Error Occured" << endl;
                        exit( 1 );
                    }

                    goto recover_label;
                }
                else // Termial; Easy
                {
                    while( pgm.empty() == false )
                    {
                        if( *pgm[0] == *symbols.top() )
                            break;
                        else
                            pgm.erase( pgm.begin() );
                    }

                    if( pgm.empty() )
                    {
                        cerr << "Error on T Recovery.\n";
                        cerr << "Unrecoverable Error Occured" << endl;
                        exit( 1 );
                    }

                    goto recover_label;
                }

                exit( 1 );
            }
        }
    }

    // Empty stack, and end of file.
    if( dynamic_cast<EndOfFileToken*>( pgm[0] ) )
    {
        if( errored )
            cerr << set_color( Red ) << "Errors occured." << set_color() << " Check output for details." << set_color() << endl;
        else
            cerr << set_color( Cyan ) << "No Errors" << set_color() << endl;
    }
    else
        cerr << set_color( Red ) << "Error: Could not consume all input tokens" << set_color() << endl;

    return root;
}
