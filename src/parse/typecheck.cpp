#include <utility>
#include <sstream>

#include <assert.h>

#include "typecheck.h"
#include "../codegen/SmartTree.h"
#include "../codegen/SmartTreeVisitor.h"

using namespace std;
using namespace ir;

ir::MainClass *mc = NULL;
ir::PrintStatement *ps = NULL;


ir::Program* TypeCheck::getIR ()
{
    return pgm;
}

TypeCheck::TypeCheck()
{
    this->global = new Context( true );
//    this->second = new Context(false);
    this->clean = true;
    this->errs = 0;
}

INode* TypeCheck::check( RTree *raw )
{
    global->enter();

    RTree *cleaned = postOrder( raw,
                                [this](RTree* t) { return this->visit( t ); },
                                [this](RTree* t) { return this->leave( t ); } );

    cleaned = postOrder( cleaned,
                                [this](RTree* t) { return this->visit2( t ); },
                                [this](RTree* t) { return this->leave2( t ); } );

    cleaned->printT();

    INode *third = buildIR( cleaned );

    //cout << "Global Namespace: " << endl;
    //global->print( 0 );

    //cout << endl << endl;

    return third;
}

RTree *TypeCheck::postOrder( RTree *tree, 
                             function<RTree* (RTree*)> visit,
                             function<RTree* (RTree*)> leave )
{
    if (!tree->isLeaf()) {
        vector< RTree* > branches = tree->getBranches();

        visit( tree );

        RTree *handle = new RTree( tree-> getVal() );

        for( RTree *branch : branches )
        {
            handle->insertSubT( postOrder( branch, visit, leave ) );
        }
        return leave( handle );
    } else {
        tree = visit( tree );
        tree = leave( tree );
        return tree;
    }
}

/**************************************************
 *
 * Third Pass starts here
 * Hopefully can merge First and Third pass, rewriting second
 * (external interface is buildIR)
 *
 *************************************************/
INode *ppostOrder( RTree *tree,
                 function<INode* (RTree*, vector<INode*>)> make_inode)
{
     vector< INode* > children;
     if (!tree->isLeaf()) {
        vector< RTree* > branches = tree->getBranches();

        for( RTree *branch : branches )
        {
            INode *processed = ppostOrder( branch, make_inode );
            if (processed) children.push_back( processed );
        }
        INode *me = make_inode( tree, children );
        return me;
    }
    return make_inode( tree, children );
}

INode *newVisit(RTree *tree, vector<INode*> children) {
    string tval = tree->printVal();
    vector<RTree*> branches = tree->getBranches();
    int deg = branches.size();
    int subs = children.size();

    BasicToken* rep = tree->getVal();

    INode *ret = NULL;

    if (tval.compare("MainClassDecl") == 0) {
        // Need to pull classs ID out here
        RTree *classname = branches[0]->getBranches()[1];
        string namestr = classname->printVal();
        cout << "MainClass name: " << namestr << endl;
        ret = new MainClass(namestr);
    } else if (tval.compare ("Program") == 0) {
        ret = new Program();
    } else if (tval.compare("ClassDecl") == 0) {
        RTree *classname = branches[0]->getBranches()[1];
        string namestr = classname->printVal();
       
        // Unfinished - make ClassDeclRHS pass up a Class type
        ir::Class *cl = dynamic_cast<ir::Class*>( children[0] );
        cl->setName(namestr);

        return cl;

    }  else if (tval.compare("ClassHeader") == 0) {
        // Just discard this
        return ret;
    } else if (tval.compare("ClassDeclRHS") == 0) {
        // Should receive a Class as a child
        // Here, we just set 'parent'

        ret = children[0];

        ir::Class *cl = dynamic_cast<ir::Class*>( ret );
        
        string extends = branches[1]->printVal();
        cl->setParentName(extends);

        return ret;
    } else if (tval.compare ("ClassBody") == 0) {
        ir::Class *cl = new ir::Class();
        ret =  cl;
        // addChild should handle the rest
    } else if (tval.compare ("ClassVarDecls") == 0) {
        

    } else if (tval.compare ("StmtLst") == 0) {
        // Stmt StmtLst <- RHS guaranteed not to be lexical
        // Stmt Stmt    <- either could be lexical blocks
        BlockStatement *bs = new BlockStatement();
        ret = bs;
    } else if (tval.compare ("Stmt") == 0) {
        string stmt_type = branches[0]->printVal();
        string pform = branches[0]->printVal();

        if (pform.compare("while") == 0) 
        {
            // Stmt -> while ( Expr ) Stmt
            ret = new WhileStatement();
        } 
        else if (pform.compare("if") == 0) 
        {
            // Stmt -> if ( Expr ) Stmt else Stmt
            ret = new IfStatement();           
        } 
        else if (pform.compare("{") == 0) 
        {
            // Input: { StmtLst } 
            return children[0];
        } 
        else if (pform.compare("System.out.println") == 0) 
        {
            ret = new PrintStatement();
        } 
        else 
        {
            // Assignments and method calls
            if (deg == 3) {
                // ID StmtRHS ;
                ret = children[1]; 
                AssignmentStatement *ass = dynamic_cast<AssignmentStatement*>( children[1] );
                FinalExpression *fx = dynamic_cast<FinalExpression*>(children[0]);
                if (ass && ass->isNew()) {
                    // We have the type here, but the assignment is complete
                    ass->setType(fx->getLiteral());
                } else if (ass && !ass->isNew()) {
                    // We cannot know the type, and we have the LHS here.
                    ass->setDest(fx->getLiteral());
                } else {
                    cerr << "FATAL: Problem in StmtRHS" << endl;
                }
                return  ret;
            } else if (deg == 5) {
                // BasicType ID = Expr ; 

                string type = branches[0]->printVal();
                string myname = branches[1]->printVal();

                AssignmentStatement *ass = new AssignmentStatement();
                ass->setDest(myname);
                ass->setType(type);
                ass->setNew(true);
                ret =  ass;


                ret->addChild(children[1]);
                return ret;
            }
        }
    } else if ( tval.compare("StmtRHS") == 0) {
        if (deg == 1) {
            // DotExpr
            return children[0];
        } else if (deg == 2){
            // We rely on the is_new flag in Stmt to figure out the variant
            // and supplly the LHS
            // Assignment. "= Expr"
            //  Cannot know the type
            AssignmentStatement *ass = new AssignmentStatement();
            ass->setNew(false);
            ass->addChild(children[0]);
            return ass;
        } else {
            // ID = Expr
            // Don't know the type! (We could if we checked the Context)
            // -> The Stmt branch will take care of it
            AssignmentStatement *ass = new AssignmentStatement();
            FinalExpression *fx = dynamic_cast<FinalExpression*>( children[0] );
            string myname = fx->getLiteral();
            ass->setDest(myname);
            ass->setNew(true);
            ass->addChild(children[1]);
            ass->addChild(children[0]);
            return ass;
        }
    } else if ( tval.compare("BoolExpr") == 0)  {
        if (subs == 1) {
            // Keep the child - this should have been trimmed earlier
            ret = children[0];
            return ret;
        } else if (subs == 2) {
            BooleanExpression *bx = dynamic_cast<BooleanExpression*>( children[1] );
            if (!bx) {
                cerr << "Fatal - Boolean expressiion received non-boolean rhs." << endl;
            } else {
                bx->addChild( children[0] );
                return bx;
            }
        } else {
            cerr << "Fatal - serious compiler error at BoolExpr clause." << endl;
        }
    } else if ( tval.compare("BoolExpr_") == 0) {
        BooleanExpression *bx = new BooleanExpression();
        BasicToken *bop;
        BooleanExpression::Operator myop;

        bop = branches[0]->getVal();

        if (subs == 1) { // end of the line
            bx->addChild(children[0]);
        } else if (subs == 2) {
            //RTree *nextadd = branches[2]->getBranches()[0];
            IExpression *rhs = dynamic_cast<IExpression*>(children[1]);
            rhs->addChild(children[0]);
            bx->addChild(rhs);
        } else {
            cerr << "FATAL MISTAKE in boolean expressionn" << endl;
            cerr << "Subtrees: " << subs << endl;
            tree->printT();
        }
        
        Operator *op = dynamic_cast<Operator*>( bop );

        switch (op->token()) {
            case And: myop = BooleanExpression::And; break;
            case Or: myop = BooleanExpression::Or; break;
            default: break; // Throw error here
        }
        bx->setOperator( myop );

        return bx;

    } else if (tval.compare("CompExpr_") == 0) {
        if (subs == 1) {
            ret = children[0];
            return ret;
        }
        // Else, boolop with AddExpr chain
        // (we only know RHS and operatioon)
        BooleanExpression *bx = new BooleanExpression();
        BasicToken *bop = branches[0]->getVal();
        BooleanExpression::Operator myop;

        Operator *op = dynamic_cast<Operator*>( bop ); 

        switch (op->token()) {
            case EqualEq: myop = BooleanExpression::Eq;  break;
            case NEqual: myop = BooleanExpression::NEq; break;
            case LT: myop = BooleanExpression::LT; break;
            case GT: myop = BooleanExpression::GT; break;
            case LEq: myop = BooleanExpression::LEq; break;
            case GEq: myop = BooleanExpression::GEq; break;
            default: break;
        }
        bx->setOperator( myop );

        bx->addChild(children[0]);

        return bx;

    }  else if (  tval.compare("AddExpr") == 0
               || tval.compare("MultExpr") == 0) {
        if (subs == 1) {
            // keep child - it's a multexpr, no +- involved
            ret = children[0];
            return ret;
        } else if (subs == 2) {
            MathExpression *mx = dynamic_cast<MathExpression*>( children[1] );
            if (!mx) {
                // Throw horrible fatal error here
                cout << "FATAL - RHS IS NOT A MATHEXPR" << endl;
            } else {
                // Add the LHS to this thing
                cout << "Atttempting to terminate MathExpr chain.." << endl;
                mx->addChild( children[0] );
                // Break out early so we don't merge incorrectly
                return mx;
            }
        } else {
            // horrible error here
            cout << "FUCK- mishandling math" << endl;
        }
    } else if (  tval.compare("AddExpr_") == 0
              || tval.compare("MultExpr_") == 0) {

        MathExpression *mx = new MathExpression();
        BasicToken *bop;
        MathExpression::Operator myop;

        bop = branches[0]->getVal();

        if (subs == 1) { // end of the line
            mx->addChild(children[0]);
        } else if (subs == 2) {
            //RTree *nextadd = branches[2]->getBranches()[0];
            IExpression *rhs = dynamic_cast<IExpression*>(children[1]);
            rhs->addChild(children[0]);
            mx->addChild(rhs);
        } else {
            cerr << "FATAL MISTAKE in mmath expressionn" << endl;
            cerr << "Subtrees: " << subs << endl;
            tree->printT();
        }
        

        Operator *op = dynamic_cast<Operator*>( bop );
        switch (op->token()) {
            case Plus: myop = MathExpression::Add;  break;
            case Minus: myop = MathExpression::Sub; break;
            case Mult: myop = MathExpression::Mul; break;
            case Div: myop = MathExpression::Div; break;
            default: break; // Throw error here
        }
        mx->setOperator( myop );

        return mx;
    } else if ( tval.compare("Literal") == 0) {
        // Literal -> ID | this | Integer | null | true | false | ( Expr )
        //          | new ID ( )

        // Only need to handle ( Expr ) and new ID ( ) here.
        ret = children[0]; 
        if (subs == 2) {
            // NewExpression
            ret->addChild(children[1]);
        }
        return ret;
    } else if ( dynamic_cast<Number*>( rep )) {
        ret = new FinalExpression(tval);
    } else if ( dynamic_cast<Identifier*>( rep )) {
        ret = new FinalExpression(tval);
    } else if ( dynamic_cast<ReservedWord*>( rep )) {
        ReservedWord* rw = dynamic_cast<ReservedWord*>( rep );
        switch (rw->token()) {
            case True:
            case False:
            case Null:
            case This: return new FinalExpression(tval); break;
            case New: return new NewExpression(); break;
            default: break;
        }
    }
  
    MainClass *mc = dynamic_cast<MainClass*>(ret);
    if (mc) cout << "MainClass detected from " << tval << endl;

    // Drop the null'd (ignored) AST nodes,
    // merge the children into the parent for the wanted nodes.
    if (ret) {
        for (INode *kid : children) {
            ret->addChild(kid);
        }
    }


    return ret;
}

INode *TypeCheck::buildIR (RTree *t) {
    return ppostOrder(t, [](RTree *input, vector <INode*> children) 
             { return newVisit(input, children); });
}



void TypeCheck::typeError (string excuse, RTree *node) {
    node->setErr();
    this->errs++;
    pair<int,int> posi = node->getPos();
    cerr << "Line " << posi.first << "c" << posi.second << ": " << excuse << endl;
    this->clean = false;
}

typedef pair<int, string> expect;

bool expectsThese(vector<expect> tups, vector<RTree*> branches) {
    bool ret = true;
    for (expect e : tups) {
        int fst = e.first;
        string exp = e.second;
        string act = branches[fst]->getType();

        if (exp.compare(act) != 0) ret = false;
    }
    return ret;
}

string TypeCheck::typeMatch (string a, string b) {
    string nil ("_nil");
    if (nil.compare(a) == 0) {
        return b;
    } else if (nil.compare(b) == 0) {
        return a;
    } else if (a.compare(b) == 0) {
        return a;
    } else {
        return "";
    }
}

string TypeCheck::matchAll (vector<RTree*> branches) 
{
    string ret = "";
    bool match = true;

    for( RTree *b: branches )
    {
        string type = b->getType();

        bool branch_nil = type.compare( "_nil" ) == 0;
        bool matches = type.compare( ret ) == 0;
        
        if( ret.empty() && !branch_nil ) 
        {
            ret = type;
        } 
        else if( !branch_nil && !matches ) 
        {
            match = false;
            //typeError("Found " + type + " when expecting " + ret, node);
        }
    }
    if( !match ) 
        ret = "";

    return ret;
}

RTree *TypeCheck::leave2( RTree *node ) {
    string tval = node->printVal();
    BasicToken* rep = node->getVal();
    vector<RTree*> branches = node->getBranches();
    int deg = branches.size();
    string match = matchAll(branches);
    bool matching = !match.empty();

    if (tval.compare ("ClassDecl") == 0
        || tval.compare("MainClassDecl") == 0)
    {
        node->setType( "_void" );
        global->leave();

        // Dreadful hack for quick println gcodegen
	    mc = new ir::MainClass(branches[1]->printVal());
        mc->addStatement(ps);
        pgm = new ir::Program();
	    pgm->setMainClass(mc);
    }
    else if( tval.compare("ClassDecls") == 0
            || tval.compare("ClassBody") == 0
            || tval.compare("Program") == 0
            || tval.compare("ClassHeader") == 0
            || tval.compare("ClassDeclRHS") == 0
            || tval.compare("ClassVarDecl") == 0 
            || tval.compare("ClassVarDecls") == 0) 
    {
        node->setType("_void");
    } else if (tval.compare ("MethodDecl") == 0) {
        // 12 public Type ID ( MethodParams ) { StmtLst return Expr ; }
        // 11 public Type ID ( ) { StmtLst return Expr ; }
        // 11 public Type ID ( MethodParams ) { return Expr ; }
        // 10 public Type ID ( ) { return Expr ; }
        // Need to ensure Type matches return Expr
        // Note: ID has been taken care of by first pass
        assert(deg >= 10);
        string expected = branches[1]->getType();
        
        int actidx;
        if (deg == 10) actidx = 7;
        else if (deg == 11) actidx = 8;
        else if (deg == 12) actidx = 9;
        else assert(false);

        string actual = branches[actidx]->getType();
        string mm = typeMatch(expected, actual);
        
        if (mm.empty()) {
            //node->setErr();
            typeError("Method does not return correct type.", node);
        }
        node->setType("_void");
        global->leave();
    } else if (tval.compare ("Stmt") == 0) {
        // Stmts always return void
        node->setType("_void");
        // Stmts have a number of different forms with different semantics
        vector<expect> tups;
        string pform = branches[0]->printVal();

        // These are pretty horrible. expectsThese accepts 2-tuples,
        // {subtreeIdx, expectedType}. Easier than picking out the
        // individual branches for inspection...
        // (We can't use matchAll on these, since the types are supposed to conflict)
        //cout << "PFORM: " << pform << endl;
        if (pform.compare("while") == 0) 
        {
            // Stmt -> while ( Expr ) Stmt
            // ensure Stmt is void
            tups = {{4, "_void"}};
            bool matches = expectsThese(tups, branches);
            if (!matches) {
                //node->setErr();
                typeError("while statement non-void.", node);
            }
        } 
        else if (pform.compare("if") == 0) 
        {
            // Stmt -> if ( Expr ) Stmt else Stmt
            tups = {{4, "_void"}, {6, "_void"}};
            bool matches = expectsThese(tups, branches);
            if (!matches) {
                //node->setErr();
                typeError("If statement branch non-void.", node);
            }
        } 
        else if (pform.compare("{") == 0) 
        {
            tups = {{1, "_void"}};
            bool matches = expectsThese(tups, branches);
            if (!matches) {
                //node->setErr();
                typeError("Braced block returning non-void.",node);
            }
        } 
        else if (pform.compare("System.out.println") == 0) 
        {
            tups = {{2, "int"}};
            bool matches = expectsThese(tups, branches);
            if (!matches) {
                //node->setErr();
                typeError("System.out.println only accepts numbers.",node);
            }

            // Dreadful and temporary hack
            // TODO: Urgently remove.
            using namespace ir;
            FinalExpression *five = new FinalExpression( branches[2]->printVal() );
            ps = new PrintStatement( five );
            //mc->addStatement( print );



        } 
        else 
        {
            // Assignments and method calls
     
            // TODO: This is broken because of DotExpr - check again after DotExpr' works properly.
            //if (deg == 3) cout << "\nTrying to declare usertype var:";
            //node->printT();
            //cout << "Degree: " << deg ;
            if (deg == 3) {
                // ID StmtRHS ;
                // We can rely on the looked-up type of ID here.
                // EDIT: now that dotexpr's type does not represent the match of
                // its subtrees, we can't use matching in all cases
                //if (!matching) {
                  //  typeError("Mismatched types in assignment.", node);
                    //node->setErr();
                //}


                string type = branches[0]->getType();
                string myname;
                BasicToken* one = branches[1]->getBranches()[0]->getVal();
                if ( dynamic_cast<Operator*>( one )) {
                    assert(0);
                } else if ( dynamic_cast<Identifier*>( one )) {
                    // ID (ID = Expr) ; <-- DeclInit  
                    myname = branches[1]->getBranches()[0]->printVal();
                    global->add(myname, type);
                    ///cout << "Declared " << myname << "::" << type << endl;
                } else if ( dynamic_cast<Delimiter*>( one )) {
                    // ID (= Expr) ; <-- Assign
                    Delimiter *del = dynamic_cast<Delimiter*>( one );
                    if (del->token() == Equal) {
                        myname = branches[0]->printVal();
                        global->add(myname, type);
                        //cout << "Decl/assed " << myname << "::" << type << endl;
                    } else if (del->token() == Period) {
                    // ID (. DotExprChain...) ; <-- method lookup
                        //cout << "<<--- Method lookup here\n";
                        resolveDexPrime(type, branches[1]);

                    }
                }
                // else fatal compiler error

            } else if (deg == 5) {
                // BasicType ID = Expr ; 

                // Blindly insert the variable name into the context table
                string type = branches[0]->printVal();
                string myname = branches[1]->printVal();
                // Ignore type of myname - allow shadowing.

                // TODO: Ensure that this fails for redefinition in scope,
                // but allows shadowing in global scope.
                global->add(myname, type);

                // Check the type of the right hand side matches BasicType
                string lhrs = typeMatch(type, branches[3]->getType());
                if (lhrs.empty()) {
                    typeError("Mismatched types in assignment.", node);
                }
            }
            //string match = matchAll(branches);
                //cout << "TVAL " << tval << endl;
                //cout << "\tType is " << match << endl;

            //if (!matching) {
            //    node->setErr(); 
            //    typeError("Mismatched types in assignment.");
            //}
            //else {
            //    global->add(tval, match);
            //}
        }
    } else if (tval.compare ("StmtLst") == 0) {
        node->setType("_void");
    } else if (tval.compare ("ExprLst") == 0) {
        // ExprLst is only ever used for function parameters in DotExpr'
        // ... we build up function parameters in the useful way
        string typeA = branches[0]->getType();
        if (deg > 1) {
            node->setType(typeA + " " + branches[1]->getType());
        } else if (typeA.compare("_void") != 0) {
            node->setType(typeA);
        } else {
            node->setType("");
        }
    } else if (tval.compare ("MethodParams") == 0
              || tval.compare ("MethodParams_") == 0
              || tval.compare ("Formal") == 0) {
        // These have been taken care of in the first pass.
        node->setType("_nil");

    } else if (tval.compare ("Type") == 0) {
        node->setType(match);
    } else if (tval.compare ("MultExpr") == 0
              || tval.compare("AddExpr") == 0
              || tval.compare("NegExpr") == 0
              || tval.compare("MultExpr_") == 0
              || tval.compare("AddExpr_") == 0
              || tval.compare("Expr") == 0
              || tval.compare("ExprLst_") == 0) {

        //bool matchAll (vector<RTree*> branches) {
        //string match = matchAll(branches);
        if (matching) 
        {
            node->setType(match);
        } 
        else 
        {
            //node->setErr();
            typeError("Type mismatch in expression.", node);
        }
    } else if (tval.compare("StmtRHS") == 0) {
        // DotExpr' <- DotExpr' becomes decideable here and should be traversed
        // = Expr <- match, allowing nil
        // ID = Expr <- must match type of ID, Expr can be nil
        if (deg == 1) {
            // DotExpr'
            // Singleton branches should have been trimmed already
            // but it is safe to trust the child's inferred type.
            node->setType(branches[0]->getType());
        } else if (deg == 2) {
            if (matching) node->setType(match);
            else node->setType("_nil");
        } else if (deg == 3) {
            if (matching) node->setType(match);
            else typeError("Mismatched types in right-hand side of statement.", node);            
        }

    } else if (tval.compare("BoolExpr_") == 0)
    {
        // Anything can be evaluated in a boolean context, which is why
        //      '||' and '&&' are both _nil typed. 
        // (... except for _void, but I don't think that is possible below
        //      boolexpr' in an AST.)
        node->setType("boolean");
    } else if (tval.compare("BoolExpr") == 0)
    {
        // CompExpr BoolExpr'
        // CompExpr
        if (deg == 2) {
            // The BoolExpr' infects everything to the left of it
            string bexpt = branches[1]->getType();
            // BoolExpr' should absolutely always be boolean
            assert(bexpt.compare("boolean") == 0);
            node->setType("boolean");
        } else if (deg == 1) {
            node->setType(branches[0]->getType());
        } else {
            // something is very broken
            assert(0);
        }
    } else if (tval.compare("CompExpr") == 0) {
        if (deg == 1) {
            node->setType(branches[0]->getType());
        } else if (deg == 2) {
            // We have a comparison. The end type is always a boolean
            node->setType("boolean");

            // ... but we need to be certain that the two arguments are of the same type
            if (!matching) {
                typeError("Comparators require like-typed expressions.", node);
            }

        } // else parsing bug

    } else if (tval.compare("CompExpr_") == 0) {
        // Inherit the type of the right subtree
        // Only ordering operators will have non-nil type information.
        // So, if there's a mismatch, can be sure someone's trying to order bools or user types.
        assert(deg == 2);
        node->setType(branches[1]->getType());
        if (!matching) {
            // < <= >= > being used on non-integer
            typeError("Ordering only defined against integers.", node);
        }
    } else if (tval.compare ("DotExpr_") == 0 || tval.compare ("DotExpr") == 0) 
    {
        // Productions handled:
        // . ID ( ExprLst  ) DotExpr_
        // . ID ( ExprLst )
        // . ID ( )
        // Literal DotExpr'
        // Literal
        //
        // Needs to handle the argument types for method lookups.

        // Argument types are concat'd in type field by ExprLst and ExprLst_
        // ... but the DotExpr or DotExpr_ to our left will need the func name
        //     in order to see if it exists.
        // Traversing these types is taken care of in Stmt, DotExpr

        // This code is a bit annoying, because we never allow epsilon productions
        //   to enter the tree. Makes it difficult to find the two relevant
        //   branches. ExprLst will be in [3] if it exists. (Degree 5)
        //   DotExpr' will be in branch [5] if we have ExprLst (Degree 6), else it will
        //   be in [4] (Degree 5)
        //   If we have neither, the degree is 4.

        string myname;
        if (deg > 2) {
            // Literal and Literal DotExpr' will not have a name
            myname = branches[1]->printVal();
            //cout << "DOTEXPR_, ID = " << myname << endl;
        }

        if (deg == 6) {
            // Both an ExprList and a DotExpr'.

            // 1. Build function and arg string
            node->setType("function " + myname + " " + branches[3]->getType());

            // 2. That's it, since we don't know our real type yet. 
            // need to visit this node later.
        } else if (deg == 5) {
            // One of the two, but not sure which.
            node->setType("function " + myname + " " + branches[3]->getType());
        } else if (deg == 4) {
            // Neither an ExprList nor a DotExpr' on the right.
            node->setType("function " + myname);
        } else if (deg == 2) {
            // Certain to be handling a DotExpr now.

            // We can look up the method here!
            dotExprResolve(node);
            //node->setType("DECIDEABLE");
            //node->setErr();
        } else if (deg == 1) {
            // Certain to be handling a DotExpr now.
            // deg 1 -> this has probably already been trimmed from the tree
            node->setType(branches[0]->getType());
        } else {
            typeError ("Method lookup error.", node);
            //node->setType("_nil");
        }

        // For nested DotExpr_ we will also need to conduct this lookup.

    }
    else if( tval.compare( "Literal" ) == 0 )
    {
        //string match = matchAll( branches );
        if( !matching )
        {
            //node->setErr();
            typeError( "Mismatched types in Literal.", node);
            // Why is this nil?
            node->setType( "_nil" );
        }
        else
            node->setType( match );
    }
    else if ( dynamic_cast<Identifier*>( rep )) 
    {
        string type = global->typeof( tval );

        if( type.find( "class" ) == 0 )
        {
            // We have something like the LHS of:
            // MyClass mc = new MyClass();
            node->setType( tval ); // Make it look like the type.
        }
        else if( type.compare( "Undefined" ) == 0 )
        {
            // We have an undefined variable. Might be "mc" of the above example.
            // We should just set it to _nil, and let the expression handle it.
            node->setType( "_nil" );
        }
        else
        {
            // Otherwise we should just set it to the lookedup type.
            node->setType( type );
        }
    } 
    else if ( dynamic_cast<Number*>( rep )) 
    {
        node->setType("int");
    } 
    else if ( dynamic_cast<Delimiter*>( rep )) {
        node->setType("_nil");
    } else if ( dynamic_cast<Operator*>( rep )) {
        Operator* op = dynamic_cast<Operator*>( rep );
        switch (op->token()) {
            // These operators can operate on any type
            //  (except for void)
            //  i.e. user types are evaluated as null or not null
            case EqualEq:  // Fall through
            case NEqual:   //      v
            case And:      //      v
            case Or:       //      v
            case Not: node->setType("_nil"); break;
            // The rest (+-/*<<=>=>) only work with numbers
            default: node->setType("int");
        };
    } else if ( dynamic_cast<ReservedWord*>( rep )) {
        ReservedWord* rw = dynamic_cast<ReservedWord*>( rep );
        switch (rw->token()) {
            case True: 
            case False:
            case Bool: node->setType("boolean"); break;
            // Null can be assigned to anything
            case Null: node->setType("_nil"); break;
            case New: node->setType("_nil"); break;

            case Int: node->setType("int" ); break;
            // TODO: Check these
            default: node->setType("_void"); break;
        };
    }

    return node;
}

RTree *TypeCheck::visit2( RTree *node ) {
    string tval = node->printVal();
    BasicToken* rep = node->getVal();

    if (tval.compare ("ClassDecl") == 0 )
    {
        string classname = node->getBranches()[0]->getBranches()[1]->printVal();
        //cout << "\n\nENTERING CLASS: " << classname << endl;

        // Add the Class namespace to this one.
        global->enter();
        global->merge( global->getNamespace( classname ) );
        
        //global->print( 0 );
    }
    else if( tval.compare("MainClassDecl") == 0 )
    {
        // Nothing else to do here; Enter for consistency
        // Main class is special and can't have functions/
        // or vatriables other than the main function.
        global->enter();

    } 
    else if( tval.compare("MethodDecl") == 0 )
    {
        // Check that the return type and argument types are defined.
        // Get the definition from the first pass
        string def = global->typeof( node->getBranches()[2]->printVal() );
        stringstream stream( def );
        string word;

        stream >> word; // Read "Function"

        while( stream >> word )
        {
            if( !( word.compare( "int" ) 
                || word.compare( "boolean" ) ) )
            {
                // Check if it's a userdefined type.
                string type = global->typeof( word );
                if( type.find( "class" ) != 0 )
                {
                    //node->setErr();
                    typeError( "Undefined class: " + type , node);
                }
            }
        }

        global->enter();

        // Add the formals in the param list to the namespace
        // [This happens automatically from the Formal case]
    }
    else if( tval.compare( "Formal" ) == 0 )
    {
        // We know this is a valid type, or at least we don't care 
        // if it's not.
        string name = node->getBranches()[1]->printVal();
        string type = node->getBranches()[0]->getBranches()[0]->printVal();

        global->add( name, type );

        //cout << "Added [" << name << " : " << type << "]" << endl;
    }
    else if( tval.compare( "ClassVarDecl" ) == 0 )
    {
        string name = node->getBranches()[1]->printVal();
        string type = node->getBranches()[0]->getBranches()[0]->printVal();

        if( type.compare( "int" ) != 0 && type.compare( "boolean" ) != 0 )
        {
            if( global->typeof( type ).compare( "Undefined" ) == 0 )
            {
                //node->setErr();
                typeError( "Error: Type " + type + " undefined.", node);
            }
        }
        global->add( name, type );

        //cout << "Added new Class Var [" << name << " : " << type << "]" << endl;
    }
    else if( tval.compare( "DotExpr" ) == 0 )
    {
        auto branches = node->getBranches();
        // Dot expressions need to know about the shit to their left. 
        // If there is actually a dot, eval it here, and ignore it later
        // otherwise eval it later.
        if( branches.size() > 1 )
        {
            // Type of a stored variable. Should give back a namespace name.
            string type = global->typeof( branches[0]->printVal() );
            // We can only dot our own types.
            if( type.compare( "int" ) == 0 ||
                type.compare( "boolean" ) == 0 )
            {
                //node->setErr();
                //typeError( "Error: builtin type " + type + " has no functions" , node);
                branches[1]->setLeftType( "Error" );
            }
            else if( type.compare( "Undefined" ) == 0 )
            {
                //node->setErr();
                //typeError( "Error: Undefined variable " + branches[0]->printVal() );
                branches[1]->setLeftType( "Error" );
            }
            else
            {
                branches[1]->setLeftType( type );
            }
        }
    }
    else if( tval.compare( "DotExpr_" ) == 0 )
    {
        // We should have "left_type" set.
        // If not, error.

        if( node->getLeftType().empty() )
        {
            //cerr << "Damnit Neil" << endl;
            //global->print();
            // Not convinced this is fatal or that left types necessary
            //exit( 2 );
        }

        if( node->getLeftType().compare( "Error" ) == 0 )
        {
            // The left side is errored, just set this to nil.
            node->setType( "_nil" );
        }
        else
        {
            // Not errored, has been set. We're good to go.
            // Need to check if the last branch is a DotExpr_,
            // and if it is, we need to set it's left_type
            // to the return value of this. Parameter type
            // checking will happen when leaving the node.

            auto branches = node->getBranches();

            // Get the function type
            string type = global->typeof( node->getLeftType(), branches[1]->printVal() );
            // Validate that it is a function
            if( type.find( "function" ) == 0 )
            {
                bool skip = true;
                stringstream stream( type );
                string temp;
                string ret_type;
                stream >> temp; // Consume "function"
                stream >> ret_type; // Get return type.

                // Can only be ) or another DotExpr_
                NonTerminal *token = dynamic_cast<NonTerminal*>( branches.back()->getVal() );

                if( token )
                    branches.back()->setLeftType( ret_type );
            }
            else
            {
                //typeError( "Error: " + node->getLeftType() + "." + branches[1]->printVal() + " is not a function" );
                node->setType( "_nil" );
            }
        }
    }

    return node;
}

void TypeCheck::dotExprResolve (RTree *t) {
    // DotExpr branches are fully decidable when leaving
    vector<RTree*> branches = t->getBranches();

    // The left subtree carries our opening namespace
    string nspace = branches[0]->getType();

    resolveDexPrime(nspace, branches[1]);
    t->setType(branches[1]->getType());

}

// TODO: Catch the other entry point for DotExpr_

// This accepts an ltype and a DotExpr_ (right now, but could be generalised)
//   it will get the munged type info from the DotExpr_, and check that function
//   exists in the namespace ltype.
void TypeCheck::resolveDexPrime (string ltype, RTree *t) {
    Context *c = global->getNamespace(ltype);

    if (!c) {
        typeError("There are no methods associated with " + ltype, t);
        return;
    }

    string munged = t->getType();
    stringstream mstrm (munged);



    string tmp;
    mstrm >> tmp;

    string fname;
    mstrm >> fname;

   // cout << "Searching namespace " << ltype << " for " << fname << endl;

    string ftype = c->typeof(fname);
    stringstream fstrm (ftype);
    fstrm >> tmp;

    //cout << "Munged: " << munged << endl;
    //cout << "Lookued up: " << ftype << endl;

    string rettype;
    fstrm >> rettype;

    // Set the return type for this node to the function ret
    t->setType(rettype);
    // This gets overridden below if the node has children

    //cout << rettype << endl;

    while (true) {
        string par1, par2;
        mstrm >> par1;
        fstrm >> par2;
        if (!par1.empty() && !par2.empty()) {
            //cout << par1 << " " << par2 << endl;
            if (par1.compare(par2) != 0) {
                typeError("Method parameters are of incorrect type.", t);
            }
        } else if (par1.empty() && par2.empty()) {
            //cout << "Successful type match!" << endl;
            break;
        } else {
            typeError("Wrong number of method parameters.", t);
            break;
        }
    }

    //cout << t->degree();

    if (t->degree() == 6) {
        RTree *b = t->getBranches()[5];
        //cout << "Proceeding to next dotExpr with ltype " << rettype << endl;
        resolveDexPrime(rettype, b);
        // Pass the type info back up
        t->setType(b->getType());
        //cout << "Does " << ltype << " == " << b->getType() << "?" << endl;
    } else {
        //cout << "Completed type match, no further dotExpr_" << endl;
    }

    // set node's type field here somewhere
    // ... and descend
}

RTree *TypeCheck::leave( RTree *node )
{
//    cout << "Enter TC::leave" << endl;

    string tval = node->printVal();
    vector< RTree* > branches = node->getBranches();

    string cmp( "Class Decl" );
    string typ( "Type" );

    if( tval.compare( "ClassDecl" ) == 0 )
    {
        //cout << "Up one level" << endl;
        //global->leave();
    }
    else if( tval.compare( "Type" ) != 0 
           && branches.size() == 1       )
    {
        return branches[0];
    }

    return node;
}

RTree *TypeCheck::visit( RTree *node )
{
    string tval = node->printVal();
    vector< RTree* > branches = node->getBranches();

    if( tval.compare( "ClassDecl" ) == 0 )
    {
        RTree *b = branches[0]->getBranches()[1];
        
        Context *classContext = new Context( true );
        classContext->enter();

        cur_namespace = string( b->printVal() );
        global->add( cur_namespace, classContext );
        global->add( cur_namespace, string( "class " ) + b->printVal() );

        //cout << "Added new Namespace: " << b->printVal() << endl;
    }
    else if( tval.compare( "ClassDeclRHS" ) == 0 )
    {
        if( branches[0]->printVal().compare( "{" ) != 0 )
        {
            // We have an extention type.
            string type = branches[1]->printVal();
            
            // Make sure it's been defined.
            if( !global->defined( type ) )
            {
                typeError( "Cannot extend undefined class " + type, node );
            }
            else
            {
                global->setType( cur_namespace, global->typeof( cur_namespace ) + " " + type );
            }
        }
    }
    else if( tval.compare( "MethodDecl" ) == 0 )
    {
        RTree *b = branches[2];
        string value;

        // Classifier
        value += "function ";
        // Return Type
        string s = branches[1]->getBranches()[0]->printVal();
        if( s.compare( "BuiltInType" ) == 0 )
            s = branches[1]->getBranches()[0]->getBranches()[0]->printVal();
        value += s;

        // Add without checking return type. Check on second pass.
        global->add( cur_namespace,
                     string( b->printVal() ),
                     value );

        cur_function = b->printVal();

        //cout << "Added new Method: " << b->printVal() << endl;
    }
    else if( tval.compare( "Formal" ) == 0 )
    {
        string type = global->typeof( cur_namespace,
                                      cur_function );

        // I dont think this should ever happen.
        if( type.compare( "Undefined" ) == 0 )
        {
            cout << "ERROR: " << cur_namespace << "::" << cur_function << " is undefined." << endl;
            exit( 1 );
        }
        
        // Two types: BuiltIn, User
        string s = branches[0]->getBranches()[0]->printVal();
        if( s.compare( "BuiltInType" ) == 0 )
            s = branches[0]->getBranches()[0]->getBranches()[0]->printVal();
        
        type += " ";
        type += s;

        global->setType( cur_namespace,
                         cur_function,
                         type );
    }
    else if( tval.compare( "ClassVarDecl" ) == 0 )
    {
/*        RTree *b = branches[1];

        string type = branches[0]->getBranches()[0]->printVal();
        if( type.compare( "BuiltInType" ) == 0 )
            type = branches[0]->getBranches()[0]->getBranches()[0]->printVal();

        global->add( cur_namespace,
                     string( b->printVal() ),
                     type );

        cout << "Added new Class Var: " << b->printVal() << endl;
*/    }

    return node;
}
