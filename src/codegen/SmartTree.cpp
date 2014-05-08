#include <iostream>
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

void Program::addChild( INode *n )
{
    MainClass *mc = dynamic_cast<MainClass*>( n );
    if (mc) {
        if (this->main_class) {
            // Program starts with something that isn't MainClass
            // Probably a parser error
            // fatalError( ); TODO: Need interface
            cerr << "FATAL: Program received two main classes." << endl;;
        } else {
            this->setMainClass(mc);
        }
    } else {
        Class *c = dynamic_cast<Class*>( n );
        if (!c) {
            // Something arbitrary got too high in the tree
            // Parser error
            // fatalError( );
            cerr << "Program can't add null pointer." << endl;
        } else {
            this->addClass(c);
        }
    }

    return;
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

void MainClass::addChild( INode *n )
{
    IStatement *s = dynamic_cast<IStatement*>( n );
    if (s) {
        this->addStatement(s);
    } else {
        // This error could probably have come from anywhere
        // fatalError( );
    }
    return;
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

void Class::addChild( INode *n )
{
    // Classes only accept members and functions
    Formal *fo = dynamic_cast<Formal*>( n );
    Function *fn = dynamic_cast<Function*>( n );
    if (fo) {
        this->addMember(fo);
    } else if (fn) {
        this->addFunction(fn);
    } else {
        // Fatal parser or decision error
        // fatalError( );
    }
    return;
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

void Formal::addChild( INode *n )
{
    // This should not be receiving children.
    // This is an ambiguous fatal error.
    // fatalError( );
    return;
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

void Function::addChild( INode *n )
{
    // Args and return type taken care of by creation procedure
    IStatement *st = dynamic_cast<IStatement*>( n );
    IExpression *ex = dynamic_cast<IExpression*>( n );
    
    if (st) {
        this->addStatement(st);
    } else if (ex) {
        this->setRet(ex);
    } else {
        // fatalError( );
    }
    return;
}


/***********************************
 *  PrintStatement Class Functions *
 ***********************************/
PrintStatement::PrintStatement( )
{
    this->value = NULL;
    enforceType( false );
}

PrintStatement::PrintStatement( IExpression *e ) 
{
    this->value = e;
    enforceType( false );
}

void PrintStatement::visit( Visitor *v )
{
    v->process( this );
}

void PrintStatement::addExpression( IExpression *e )
{
    //if (this->value) fatalError();
    this->value = e;
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

void MathExpression::addChild( INode *n )
{
    return;
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

void CallExpression::addChild( INode *n )
{
    return;
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

void NewExpression::addChild( INode *n )
{
    return;
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

void FinalExpression::addChild( INode *n )
{
    return;
}

} // End Namespace <ir>
