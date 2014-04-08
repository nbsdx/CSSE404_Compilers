
#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <unistd.h>

#include "tt.hpp"
#include "../lex/lexer.h"
#include "AST.h"

using namespace std;

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

int main( int argc, char **argv )
{
    // Initialize our Transition Table.
    TransitionTable tt;
    vector<BasicToken*>::reverse_iterator rit;
    int fd;

    if( argc != 2 )
    {
        std::cout << "Error please supply file\n";
        exit( 1 );
    }

    if( ( fd = open( argv[1], O_RDONLY ) ) == -1 )
    {
        std::cout << "Error opening file\n";
        exit( 1 );
    }

    // Lexer Output.
    vector<BasicToken*> pgm = lex::Lexer().lex( fd );

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
        //cout << "Pushing: "; print_token( *rit );
        symbols.push( *rit );
    }

    curProdGroup = 1;

    // Loop
    while( symbols.size() > 0 )
    {
        //assert(!tree.empty());

        //cout << "Testing: "; print_token( symbols.top() );
        //cout << "Against: "; print_token( pgm[0] );

        if( dynamic_cast<Epsilon*>( symbols.top() ) )
        {
            symbols.pop();
            BasicToken *nt = symbols.top();
            //tree.pop();
            //symbols.push(new Separator() );
        }
        else if( dynamic_cast<EndOfFileToken*>( symbols.top() ) )
        {
            cout << "Finished!" << endl;
            pgm.erase( pgm.begin() );
            symbols.pop();
        }
        else if( dynamic_cast<EndOfStack*>( symbols.top() ) )
        {
            cout << "Hit bottom of stack." << endl;
            symbols.pop();
        }
        else if ( dynamic_cast<Separator*>( symbols.top() ) )
        {
            symbols.pop();
            cout << "Merging trees - hit separator\n";
            cout << "tstack size - " << tree.size() << "\n";
            cout << "Stack top - " << tree.top()->printVal() << "\n";
            if (tree.size() == 1) {
                // Hit bottom of stack
                //tree.top()->printT();
            } else {
                RTree *sub = tree.top();
                tree.pop();
                if (!sub->isLeaf() && sub->degree() > 0)
                  tree.top()->insertSubT( sub );
            }
        }
        else if( match( symbols.top(), pgm[0] ) )
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

                Separator *sep = new Separator();
                symbols.push( sep );

                int prod_group_id = tt.prodGroupId( nt->raw() );
                //cout << "ProdGroupID:  " << prod_group_id << endl;

                int next_prod_index;

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
                for( rit = temp.rbegin(); rit != temp.rend(); ++rit )
                {
                    symbols.push( *rit );
                }
            }
            else
            {
                cout << "Error parsing Token: " << pgm.front()->raw() << endl;
                cout << "\tExpected: " << symbols.top()->raw() << endl;
                exit( 1 );
            }
        }
    }

    root->printT();

    return 0;
}
