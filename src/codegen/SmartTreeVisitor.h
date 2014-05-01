
#ifndef SMARTTREEVISITOR_H
#define SMARTTREEVISITOR_H

#include "SmartTree.h"

#include <fstream>

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
    fstream file;
public:
    CodeGenerator( const string& );
    void process( Program * );
    void process( MainClass * );
    void process( Class * );
    void process( Function * );
    void process( Formal * );
    void process( PrintStatement * );
    void process( FinalExpression * );
};

} // End Namespace <ir>

#endif
