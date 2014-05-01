
#ifndef SMARTTREEVISITOR_H
#define SMARTTREEVISITOR_H

#include "SmartTree.h"

#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <map>

namespace ir
{

class Visitor
{
public:
    virtual void process( Program * ) = 0;
    virtual void process( MainClass * ) = 0;
    virtual void process( Class * ) = 0;
    virtual void process( Function * ) = 0;
    virtual void process( Formal * ) = 0;
    virtual void process( PrintStatement * ) = 0;
    virtual void process( FinalExpression * ) = 0;

    void visitIStatement( IStatement * );
    void visitIExpression( IExpression * );
};

class PrintVisitor : public Visitor
{
public:
    void process( Program * );
    void process( MainClass * );
    void process( Class * );
    void process( Function * );
    void process( Formal * );
    void process( PrintStatement * );
    void process( FinalExpression * );
};

class CodeGenerator : public Visitor
{
public:
    enum RegisterState
    {
        Clean,
        InUse,
        Dirty
    };

    CodeGenerator( const string& );
    void process( Program * );
    void process( MainClass * );
    void process( Class * );
    void process( Function * );
    void process( Formal * );
    void process( PrintStatement * );
    void process( FinalExpression * );

private:

    void finalize_function();
    void finalize_program();

    fstream file;
    stack<string> outreg;
    stack<string> available_registers;
    map<string,RegisterState> register_state;

    // Function parts.
    stringstream function_header;
    stringstream function_footer;
    stringstream function_body;

    // Sections
    stringstream section_data;

    // globals/externs
    stringstream text_header;
    stringstream text_body;
};

} // End Namespace <ir>

#endif
