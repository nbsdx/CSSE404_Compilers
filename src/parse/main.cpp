
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
#include "context.h"
#include "typecheck.h"
#include "parse.h"
#include "../codegen/SmartTree.h"
#include "../codegen/SmartTreeVisitor.h"


using namespace std;
using namespace ir;

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

    Parser p;

    RTree *raw = p.parse( pgm );

    TypeCheck *tc = new TypeCheck();

    INode *checked = tc->check( raw );

    if (tc->clean) {
        cerr << "Passed type checking. Compilation can proceed.\n";
    } else {
        cerr << tc->errs << " type error(s) detected. Compilation must halt.\n";
    }





    // holy duplication
    if( ( argc > 2 ) && ( string( "--print" ).compare( argv[2] ) == 0 ) )
    {
        //checked->printT();
        PrintVisitor *visitor = new PrintVisitor();
        checked->visit( visitor );
        Context *gc = tc->global;
        CodeGenerator *gen = new CodeGenerator( "out", gc );
        Program *prgm = dynamic_cast<Program*>( checked  );
        if (prgm) cout << "IS A PROGRAM" << endl;
        MainClass *mc = dynamic_cast<MainClass*>( checked  );
        if (mc) { cout << "IS A MAIINCLASS" << endl;
                  // How is this happening exactly
                  // should probably figure it out soon because we don't get code without Program
                  prgm = new Program();
                  prgm->addChild(mc); 
                };
        prgm->visit( gen );

        //cout << endl;
    } else cerr << "Run with --print to see AST.\n";


    if (tc->clean) {
        if ( ( argc > 2 ) && ( string( "--asm" ).compare( argv[2] ) == 0) ) {
            Context *gc = tc->global;
            if (argc != 4) {
                cerr << "Please supply a file to write to." << endl;
                return EXIT_FAILURE;
            } else {
                ///CodeGenerator *gen = new CodeGenerator( argv[3], gc );
                //CodeGenerator *gen = new CodeGenerator( argv[3], gc );
                //checked->visit( gen );
            }
        } else {
            return EXIT_SUCCESS;
        }
    } else {
        return EXIT_FAILURE;
    }

}
