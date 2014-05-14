
#ifndef SMARTTREE_H
#define SMARTTREE_H 1

#include <string>
#include <vector>

namespace ir
{

using namespace std;

class Visitor;
class INode
{
protected:
    string  p_type;
    bool    p_force_type;

public:
    virtual void addChild( INode *n ) = 0;
    virtual void visit( Visitor * ) = 0;

    void setNodeType( const string &s ){ p_type = s; }
    void enforceType( bool b ){ p_force_type = b; }

    string getNodeType(){ return p_type; }
    bool isTypeEnforced(){ return p_force_type; }
};


class Program;
class MainClass;
class Class;
class Function;
class Formal;

class IExpression : public INode
{
    string type;

public:
    virtual void visit( Visitor * ) = 0;
    void addChild( INode *n ) = 0;

    void setType( const string &type ){ this->type = type; }
    string getType(){ return this->type; }
};

class IStatement : public INode
{
public:
    virtual void visit( Visitor * ) = 0;
    void addChild( INode *n ) = 0;
};

class Program : public INode
{
    MainClass *main_class;
    vector<Class*> classes;

public:
    Program();
    void addChild( INode *n );
    void addClass( Class * );
    void setMainClass( MainClass * );
    void visit( Visitor * );

    MainClass *getMainClass(){ return main_class; }
    vector<Class*> getClasses(){ return classes; }
};

class MainClass : public INode
{
    string name;
    vector<IStatement*> body;

public:
    MainClass( const string& );
    void addStatement( IStatement * );
    void visit( Visitor * );
    void addChild( INode *n );

    string getName(){ return name; }
    vector<IStatement*> getBody(){ return body; }
};

class Class : public INode
{
    string name;
    string parent_name;
    Class *parent;
    vector<Formal*> members;
    vector<Function*> functions;

public:
    Class(  );
    void addMember( Formal * );
    void addFunction( Function * );
    void visit( Visitor * );
    void setName( const string& );
    void setParent( Class *c );
    void setParentName( const string& );
    void addChild( INode *n );

    string getName(){ return name; }
    vector<Formal*> getMembers(){ return members; }
    vector<Function*> getFunctions(){ return functions; }
    Class *getParent(){ return parent; }
    string getParentName(){ return parent_name; }
};

class Formal : public INode
{
    string type;
    string name;

public:
    Formal( const string &type, const string &name );

    string getName(){ return name; }
    string getType(){ return type; }
    void visit( Visitor * );
    void addChild( INode *n );
};

class Function : public INode
{
    string name;
    string type;
    vector<Formal*> args;
    vector<IStatement*> body;
    IExpression *ret;

public:
    Function( const string& );
    void setRetType( const string& );
    void addArg( Formal * );
    void addStatement( IStatement * );
    void setRet( IExpression * );
    void visit( Visitor * );
    void addChild( INode *n );

    string getName(){ return name; }
    string getRetType(){ return type; }
    IExpression *getRet(){ return ret; }
    vector<Formal*> getArgs(){ return args; }
    vector<IStatement*> getBody(){ return body; }
};


class PrintStatement : public IStatement
{
    IExpression *value;

public:
    PrintStatement( );
    PrintStatement( IExpression * );
    void visit( Visitor * );
    void addChild( INode *n );
    void addExpression( IExpression *e );

    IExpression *getValue(){ return value; }
};

class BlockStatement : public IStatement
{
    vector<IStatement*> body;

public:
    BlockStatement( );
    void addChild( INode *n );
    vector<IStatement*> getBody( );
    void visit( Visitor *v );
};



class AssignmentStatement : public IStatement
{
    string dest;
    string type;
    bool   is_new;
    IExpression *value;

public:
    AssignmentStatement();
    void setDest( const string& );
    void setType( const string& );
    void setValue( IExpression * );
    void setNew( bool );
    void visit( Visitor * );
    void addChild( INode *n );
    bool isNew();

    string getDest(){ return dest; }
    string getType(){ return type; }
    bool getNew(){ return is_new; }
    IExpression *getValue(){ return value; }
};

class IfStatement : public IStatement
{
    vector<IStatement*> on_true;
    vector<IStatement*> on_false;
    IExpression *condition;
    bool has_true;
    bool has_false;

public:
    IfStatement();
    void addTrueStatement( IStatement * );
    void addFalseStatement( IStatement * );
    void setCondition( IExpression * );
    void visit( Visitor * );
    void addChild( INode *n );

    vector<IStatement*> getOnTrue(){ return on_true; }
    vector<IStatement*> getOnFalse(){ return on_false; }
    IExpression *getCondition(){ return condition; }
};

class WhileStatement : public IStatement
{
    vector<IStatement*> body;
    IExpression *condition;

public:
    WhileStatement();
    void setCondition( IExpression * );
    void addStatement( IStatement * );
    void visit( Visitor * );
    void addChild( INode *n );

    IExpression *getCondition(){ return condition; }
    vector<IStatement*>getBody(){ return body; }
};

class MathExpression : public IExpression
{
public:
    enum Operator
    {
        Add,
        Sub,
        Mul,
        Div
    };

private:
    IExpression *left;
    IExpression *right;
    Operator op;

public:
    MathExpression();
    void setLeft( IExpression * );
    void setRight( IExpression * );
    void setOperator( Operator );
    void visit( Visitor * );
    void addChild( INode *n );

    IExpression *getLeft(){ return left; }
    IExpression *getRight(){ return right; }
    Operator getOperator(){ return op; }
};

class BooleanExpression : public IExpression
{
public:
    enum Operator
    {
        Eq,
        NEq,
        LEq,
        GEq,
        LT,
        GT,
        And,
        Or
    };

    BooleanExpression();
    void setLeft( IExpression * );
    void setRight( IExpression * );
    void visit( Visitor * );
    void addChild( INode *n );
    void setOperator(Operator o);
    IExpression *getLeft(){ return left; }
    IExpression *getRight(){ return right; }
    Operator getOperator(){ return op; }
   
private:
    IExpression *left;
    IExpression *right;
    Operator op;
};

class CallExpression : public IExpression
{
    IExpression *caller;
    string class_name;
    string function_name;
    vector<IExpression*> args;

public:
    CallExpression();
    void setCaller( IExpression * );
    void setClass( const string& );
    void setFunction( const string& );
    void addArgument( IExpression * );
    void visit( Visitor * );
    void addChild( INode *n );

    IExpression *getCaller(){ return caller; }
    string getClass(){ return class_name; }
    string getFunction(){ return function_name; }
    vector<IExpression*> getArgs(){ return args; }
};

class NewExpression : public IExpression
{
    string class_name;

public:
    NewExpression( );
    void visit( Visitor * );
    void addChild( INode *n );

    string getClass(){ return class_name; }
};

class FinalExpression : public IExpression
{
    string literal;

public:
    FinalExpression( const string& );
    void visit( Visitor * );
    void addChild( INode *n );

    string getLiteral(){ return literal; }
};

} // End Namespace <ir>

#endif
