#include <iostream> // Temporary includes for debugging
#include <assert.h> // during CodeGen phase

#include "SmartTree.h"
#include "SmartTreeVisitor.h"

namespace ir
{

/****************************
 *  Program Class Functions *
 ****************************/
Program::Program()
{
    this->main_class = NULL;
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
    BlockStatement *bs = dynamic_cast<BlockStatement*>( n );
    IStatement *s = dynamic_cast<IStatement*>( n );
    
    if (bs) {
        cerr << "Visiting a BlockStatement."  << endl;
        vector<IStatement*> sts = bs->getBody();
        for (IStatement *st : sts) {
            this->addChild(st);
        }
    } else if (s) {
        this->addStatement(s);
    } else {
        cerr << "FATAL: MainClass received non-statement." << endl;
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
 *  BlockStatement Class Functions *
 ****************************************/
BlockStatement::BlockStatement( )
{

}

void BlockStatement::visit( Visitor *v )
{
    v->process( this );
}

void BlockStatement::addChild( INode *n )
{
    IStatement *ist = dynamic_cast<IStatement*>( n );
    if (!ist) {
        cerr << "INVALID STATEMENT ENTERED BLOCK" << endl;
    }
    this->body.push_back(ist);
}

vector<IStatement*> BlockStatement::getBody( )
{
    return this->body;
}

/****************************************
 *  AssignmentStatement Class Functions *
 ****************************************/
AssignmentStatement::AssignmentStatement()
{
    this->dest = "";
    this->type = "";
    this->is_new = false;
    this->value = NULL;
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

void AssignmentStatement::addChild( INode *n )
{
    IExpression *ie = dynamic_cast<IExpression*>( n );

    if (!ie) {
        cerr << "FATAL - Invalid value at assignment" <<  endl;
    } else if (this->value) {
        cerr << "FATAL - ASSIGNMENT GIVEN TWO EXPRS" << endl;
    }

    this->value = ie;
}

bool AssignmentStatement::isNew()
{
    return this->is_new;
}

/********************************
* IfStatement Class Functions *
********************************/
IfStatement::IfStatement()
{
    condition = nullptr;
    has_true = false;
    has_false = false;
}

void IfStatement::addTrueStatement( IStatement *s )
{
    this->on_true.push_back( s );
    has_true = true;
}

void IfStatement::addFalseStatement( IStatement *s )
{
    this->on_false.push_back( s );
    has_false = true;
}

void IfStatement::setCondition( IExpression *c )
{
    this->condition = c;
}

void IfStatement::visit( Visitor *v )
{
    v->process( this );
}

void IfStatement::addChild( INode *n )
{
    IExpression *ie = dynamic_cast<IExpression*>( n );    
    BlockStatement *bs = dynamic_cast<BlockStatement*>( n );
    IStatement *st = dynamic_cast<IStatement*>( n );
    if (!condition) {
        if (ie) {
            this->setCondition( ie );
        } else {
            cerr << "If statement has bad condition." << endl;
            assert(false);
        }
    } else if (!has_true) {
        if (bs) {
            vector<IStatement*> body = bs->getBody();
            this->on_true = body;
            this->has_true = true;
        } else if (st) {
            this->addTrueStatement(st);
        } else {
            cerr << "If statement got bad true branch." << endl;
        }
    } else if (!has_false) {
        if (bs) {
            vector<IStatement*> body = bs->getBody();
            this->on_false = body;
            this->has_false = true;
        } else if (st) {
            this->addFalseStatement(st);
        } else {
            cerr << "If statement got bad true branch." << endl;
        }
    } else {
        cerr << "If statement received invalid input." << endl;
    }
}

/***********************************
* WhileStatement Class Functions *
***********************************/
WhileStatement::WhileStatement()
{
    this->condition = nullptr;
}

void WhileStatement::setCondition( IExpression *c )
{
    this->condition = c;
}

void WhileStatement::addStatement( IStatement *s )
{
    this->body.push_back( s );
}

void WhileStatement::visit( Visitor *v )
{
    v->process( this );
}

void WhileStatement::addChild( INode *n )
{
    IExpression *ie = dynamic_cast<IExpression*>( n );    
    BlockStatement *bs = dynamic_cast<BlockStatement*>( n );
    IStatement *st = dynamic_cast<IStatement*>( n );
    if (!this->condition) {
        if (ie) {
            this->setCondition(ie);
        } else {
            cerr << "While statement received bad condition." << endl;
        }
    } else {
        if (bs) {
            this->body = bs->getBody();
        } else if (st) {
            this->addStatement(st);
        }  else {
            cerr << "While statement received bad body." << endl;
        }
    }
}

/***********************************
 *  MathExpression Class Functions *
 ***********************************/
MathExpression::MathExpression()
{
    this->left = NULL;
    this->right = NULL;
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
    IExpression *ie = dynamic_cast<IExpression*>( n );
    if (!ie) {
        // Horrible error
        cerr << "MathExpression  received a non-expression" << endl;
        return;
    }
    if (!this->right) {
        this->right = ie;
    } else if (!this->left) {
        this->left = ie;
    } else {
        // Horrible error
        cerr << "MathExpr received more than two  branches..." << endl;
    }
    return;
}

/***********************************
 *  BooleanExpression Class Functions *
 ***********************************/
BooleanExpression::BooleanExpression()
{
    this->left = NULL;
    this->right = NULL;
}

void BooleanExpression::setLeft( IExpression *l )
{
    this->left = l;
}

void BooleanExpression::setRight( IExpression *r )
{
    this->right = r;
}

void BooleanExpression::setOperator( Operator o )
{
    this->op = o;
}

void BooleanExpression::visit( Visitor *v )
{
    v->process( this );
}

void BooleanExpression::addChild( INodee *n )
{
    IExpression *ie = dynamic_cast<IExpression*>( n );
    if (!ie) {
        // Horrible error
        cerr << "BoolExpression  received a non-expression" << endl;
        return;
    }
    if (!this->right) {
        this->right = ie;
    } else if (!this->left) {
        this->left = ie;
    } else {
        // Horrible error
        cerr << "BoolExpr received more than two  branches..." << endl;
    }
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
NewExpression::NewExpression( )
{
}

void NewExpression::visit( Visitor *v )
{
    v->process( this );
}

void NewExpression::addChild( INode *n )
{
    FinalExpression *fx = dynamic_cast<FinalExpression*>( n );
    if (fx) {
        string cn = fx->getLiteral();
        this->class_name = cn;
    } else {
        cerr << "FATAL: NewExpression did not receive a classname." << endl;
        assert(false);
    }
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
