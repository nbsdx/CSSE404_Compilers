
#include "SmartTree.h"
#include "SmartTreeVisitor.h"

namespace ir
{

/****************************
 *  Program Class Functions *
 ****************************/
Program::Program()
{
    enforceType( false );
}

void Program::addClass( Class *c )
{
    this->classes.push_back( c );
}

void Program::setMainClass( MainClass *mc )
{
    this->main_class = mc;
}

void Program::visit( Visitor *v )
{
    v->process( this );
}

/******************************
 *  MainClass Class Functions *
 ******************************/
MainClass::MainClass( const string &name )
{
    this->name = name;
    enforceType( false );
}

void MainClass::addStatement( IStatement *s )
{
    this->body.push_back( s );
}

void MainClass::visit( Visitor *v )
{
    v->process( this );
}

/**************************
 *  Class Class Functions *
 **************************/
Class::Class( const string &name )
{
    this->name = name;
    this->parent = nullptr;
    this->parent_name = "";
    enforceType( false );
}

void Class::setParent( Class *p )
{
    this->parent = p;
}

void Class::setParentName( const string &n )
{
    this->parent_name = n;
}

void Class::addMember( Formal *f )
{
    this->members.push_back( f );
}

void Class::addFunction( Function *f )
{
    this->functions.push_back( f );
}

void Class::visit( Visitor *v )
{
    v->process( this );
}

/***************************
 *  Formal Class Functions *
 ***************************/
Formal::Formal( const string &type, const string &name )
{
    this->type = type;
    this->name = name;
    enforceType( false );
}

void Formal::visit( Visitor *v )
{
    v->process( this );
}

/*****************************
 *  Function Class Functions *
 *****************************/
Function::Function( const string &name )
{
    this->name = name;
    enforceType( false );
}

void Function::setRetType( const string &type )
{
    this->type = type;
}

void Function::addArg( Formal *f )
{
    this->args.push_back( f );
}

void Function::addStatement( IStatement *s )
{
    this->body.push_back( s );
}

void Function::setRet( IExpression *s )
{
    this->ret = s;
}

void Function::visit( Visitor *v )
{
    v->process( this );
}

/***********************************
 *  PrintStatement Class Functions *
 ***********************************/
PrintStatement::PrintStatement( IExpression *e ) 
{
    this->value = e;
    enforceType( false );
}

void PrintStatement::visit( Visitor *v )
{
    v->process( this );
}

void PrintStatement::addChild( INode *n )
{
    IExpression *ex = dynamic_cast<IExpression*>( n );
    if (ex) {
        // TODO: Refactor to addExpression
        // Not so sure about this - can't use constructor?
        this->addExpression( ex );
    } else {
        //  fatalError(  );
    }
    return;
}

/****************************************
 *  AssignmentStatement Class Functions *
 ****************************************/
AssignmentStatement::AssignmentStatement()
{
    this->dest = "";
    this->type = "";
    this->is_new = false;
    this->value = nullptr;
}

void AssignmentStatement::setType( const string &t )
{
    this->type = t;
}

void AssignmentStatement::setDest( const string &d )
{
    this->dest = d;
}

void AssignmentStatement::setValue( IExpression *v )
{
    this->value = v;
}

void AssignmentStatement::setNew( bool b )
{
    this->is_new = b;
}

void AssignmentStatement::visit( Visitor *v )
{
    v->process( this );
}


/***********************************
 *  MathExpression Class Functions *
 ***********************************/
MathExpression::MathExpression()
{
    
}

void MathExpression::setLeft( IExpression *l )
{
    this->left = l;
}

void MathExpression::setRight( IExpression *r )
{
    this->right = r;
}

void MathExpression::setOperator( Operator o )
{
    this->op = o;
}

void MathExpression::visit( Visitor *v )
{
    v->process( this );
}

/***********************************
 *  CallExpression Class Functions *
 ***********************************/
CallExpression::CallExpression()
{

}

void CallExpression::setClass( const string &cname )
{
    this->class_name = cname;
}

void CallExpression::setCaller( IExpression *exp )
{
    this->caller = exp;
}

void CallExpression::setFunction( const string &fname )
{
    this->function_name = fname;
}

void CallExpression::addArgument( IExpression *exp )
{
    this->args.push_back( exp );
}

void CallExpression::visit( Visitor *v )
{
    v->process( this );
}

/**********************************
 *  NewExpression Class Functions *
 **********************************/
NewExpression::NewExpression( const string &cname )
{
    this->class_name = cname;
}

void NewExpression::visit( Visitor *v )
{
    v->process( this );
}

/************************************
 *  FinalExpression Class Functions *
 ************************************/
FinalExpression::FinalExpression( const string &lit )
{
    this->literal = lit;
}

void FinalExpression::visit( Visitor *v )
{
    v->process( this );
}

} // End Namespace <ir>
