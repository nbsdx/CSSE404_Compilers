
#include "SmartTree.h"
#include "SmartTreeVisitor.h"

namespace ir
{

/****************************
 *  Program Class Functions *
 ****************************/
Program::Program()
{

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
}

void PrintStatement::visit( Visitor *v )
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
