
#ifndef SMARTTREEVISITOR_H
#define SMARTTREEVISITOR_H

#include "SmartTree.h"
#include "../parse/context.h"

#include <utility>
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
    virtual void process( AssignmentStatement * ) = 0;
    virtual void process( IfStatement * ) = 0;
    virtual void process( WhileStatement * ) = 0;
    virtual void process( FinalExpression * ) = 0;
    virtual void process( NewExpression * ) = 0;
    virtual void process( CallExpression * ) = 0;
    virtual void process( MathExpression * ) = 0;
    virtual void process( BooleanExpression * ) = 0;
    virtual void process( BlockStatement * ) = 0;

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
    void process( AssignmentStatement * );
    void process( IfStatement * );
    void process( WhileStatement * );
    void process( FinalExpression * );
    void process( NewExpression * );
    void process( CallExpression * );
    void process( MathExpression * );
    void process( BooleanExpression * );
    void process( BlockStatement * );
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

    CodeGenerator( const string&, Context * );
    void process( Program * );
    void process( MainClass * );
    void process( Class * );
    void process( Function * );
    void process( Formal * );
    void process( PrintStatement * );
    void process( AssignmentStatement * );
    void process( IfStatement * );
    void process( WhileStatement * );
    void process( FinalExpression * );
    void process( NewExpression * );
    void process( CallExpression * );
    void process( MathExpression * );
    void process( BooleanExpression * );
    void process( BlockStatement * );

private:

    void finalize_function();
    void finalize_class();
    void finalize_program();
    void reserve_register();
    void release_register();

    fstream file;
    stack<string> outreg;
    stack<string> available_registers;
    map<string,RegisterState> register_state;
    

    // classname, {varname, varoffset}
    map<string, vector<pair<string,size_t>>* > class_members;

    Context *context;

    map<string, string> functions;

    map<string, size_t> function_locals;
    map<string, size_t>::iterator last_local;

    // Class marker.
    string current_class;
    string current_function;

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
