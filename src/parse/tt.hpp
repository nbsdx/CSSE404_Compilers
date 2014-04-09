
/**
    AUTO GENERATED FILE DO NOT MODIFY
**/

#ifndef TRANSITION_TABLE_H
#define TRANSITION_TABLE_H 1

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <memory>

#include "../util/token.h"

using namespace std;

struct Production
{
    string
            lhs;
    vector<vector<BasicToken*>>
            rhs;
    int
            g_id;

    Production( string lhs,
                vector<vector<BasicToken*>> rhs,
                int g_id )
    {
        this->lhs = lhs;
        this->rhs = rhs;
        this->g_id = g_id;
    }

    Production(){}
};

class TransitionTable
{
    vector< Production > productions;
    vector< string >     prod_names;
    vector< map< string, int > > table;
    map< string, vector<string>> firstset;

    int indexOf( const vector<string> &vec, const string& value ) {
        vector<string>::const_iterator it;
        int index;

        for( index = 0, it = vec.begin(); it != vec.end(); ++it, ++index )
        {
            if( value.compare( *it ) == 0 )
                return index;
        }

        return -1;
    }

public:
    TransitionTable()
    {
        vector<vector<BasicToken*>> tvec;
        map<string, int> tmap;
        // Production creation goes here.
		prod_names.push_back( "Program" );
		prod_names.push_back( "MainClassDecl" );
		prod_names.push_back( "ClassDecl" );
		prod_names.push_back( "ClassHeader" );
		prod_names.push_back( "ClassDeclRHS" );
		prod_names.push_back( "ClassBody" );
		prod_names.push_back( "MaybeMethods" );
		prod_names.push_back( "MaybeMehtods" );
		prod_names.push_back( "ClassDecls" );
		prod_names.push_back( "ClassVarDecl" );
		prod_names.push_back( "ClassVarDecls" );
		prod_names.push_back( "MethodDecl" );
		prod_names.push_back( "MethodDecls" );
		prod_names.push_back( "MethodParams" );
		prod_names.push_back( "MethodParams_" );
		prod_names.push_back( "Formal" );
		prod_names.push_back( "Type" );
		prod_names.push_back( "BuiltInType" );
		prod_names.push_back( "Stmt" );
		prod_names.push_back( "StmtRHS" );
		prod_names.push_back( "StmtLst" );
		prod_names.push_back( "Expr" );
		prod_names.push_back( "BoolExpr" );
		prod_names.push_back( "BoolExpr_" );
		prod_names.push_back( "CompExpr" );
		prod_names.push_back( "CompExpr_" );
		prod_names.push_back( "AddExpr" );
		prod_names.push_back( "AddExpr_" );
		prod_names.push_back( "MultExpr" );
		prod_names.push_back( "MultExpr_" );
		prod_names.push_back( "NegExpr" );
		prod_names.push_back( "Constant" );
		prod_names.push_back( "Literal" );
		prod_names.push_back( "DotExpr" );
		prod_names.push_back( "ExprLst" );
		prod_names.push_back( "ExprLst_" );
	// Production: 0
		tvec = {
			{ this->FromString( "MainClassDecl" ), this->FromString( "ClassDecls" ),  },
		};
 		productions.push_back( Production( "Program", tvec, 0 ) );
		tvec.clear();
	// Production: 1
		tvec = {
			{ this->FromString( "ClassHeader" ), this->FromString( "{" ), this->FromString( "public" ), this->FromString( "static" ), this->FromString( "void" ), this->FromString( "main" ), this->FromString( "(" ), this->FromString( "String" ), this->FromString( "[" ), this->FromString( "]" ), this->FromString( "ID" ), this->FromString( ")" ), this->FromString( "{" ), this->FromString( "StmtLst" ), this->FromString( "}" ), this->FromString( "}" ),  },
		};
 		productions.push_back( Production( "MainClassDecl", tvec, 1 ) );
		tvec.clear();
	// Production: 2
		tvec = {
			{ this->FromString( "ClassHeader" ), this->FromString( "ClassDeclRHS" ),  },
		};
 		productions.push_back( Production( "ClassDecl", tvec, 2 ) );
		tvec.clear();
	// Production: 3
		tvec = {
			{ this->FromString( "class" ), this->FromString( "ID" ),  },
		};
 		productions.push_back( Production( "ClassHeader", tvec, 3 ) );
		tvec.clear();
	// Production: 4
		tvec = {
			{ this->FromString( "extends" ), this->FromString( "{" ), this->FromString( "ClassBody" ), this->FromString( "}" ),  },
			{ this->FromString( "{" ), this->FromString( "ClassBody" ), this->FromString( "}" ),  },
		};
 		productions.push_back( Production( "ClassDeclRHS", tvec, 5 ) );
		tvec.clear();
	// Production: 6
		tvec = {
			{ this->FromString( "ClassVarDecls" ), this->FromString( "MaybeMethods" ),  },
			{ this->FromString( "MethodDecls" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "ClassBody", tvec, 8 ) );
		tvec.clear();
	// Production: 9
		tvec = {
			{ this->FromString( "MethodDecls" ),  },
		};
 		productions.push_back( Production( "MaybeMethods", tvec, 9 ) );
		tvec.clear();
	// Production: 10
		tvec = {
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "MaybeMehtods", tvec, 10 ) );
		tvec.clear();
	// Production: 11
		tvec = {
			{ this->FromString( "ClassDecl" ), this->FromString( "ClassDecls" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "ClassDecls", tvec, 12 ) );
		tvec.clear();
	// Production: 13
		tvec = {
			{ this->FromString( "Type" ), this->FromString( "ID" ), this->FromString( ";" ),  },
		};
 		productions.push_back( Production( "ClassVarDecl", tvec, 13 ) );
		tvec.clear();
	// Production: 14
		tvec = {
			{ this->FromString( "ClassVarDecl" ), this->FromString( "ClassVarDecls" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "ClassVarDecls", tvec, 15 ) );
		tvec.clear();
	// Production: 16
		tvec = {
			{ this->FromString( "public" ), this->FromString( "Type" ), this->FromString( "ID" ), this->FromString( "(" ), this->FromString( "MethodParams" ), this->FromString( ")" ), this->FromString( "{" ), this->FromString( "StmtLst" ), this->FromString( "return" ), this->FromString( "Expr" ), this->FromString( ";" ), this->FromString( "}" ),  },
		};
 		productions.push_back( Production( "MethodDecl", tvec, 16 ) );
		tvec.clear();
	// Production: 17
		tvec = {
			{ this->FromString( "MethodDecl" ), this->FromString( "MethodDecls" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "MethodDecls", tvec, 18 ) );
		tvec.clear();
	// Production: 19
		tvec = {
			{ this->FromString( "Formal" ), this->FromString( "MethodParams_" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "MethodParams", tvec, 20 ) );
		tvec.clear();
	// Production: 21
		tvec = {
			{ this->FromString( "," ), this->FromString( "Formal" ), this->FromString( "MethodParams_" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "MethodParams_", tvec, 22 ) );
		tvec.clear();
	// Production: 23
		tvec = {
			{ this->FromString( "Type" ), this->FromString( "ID" ),  },
		};
 		productions.push_back( Production( "Formal", tvec, 23 ) );
		tvec.clear();
	// Production: 24
		tvec = {
			{ this->FromString( "BuiltInType" ),  },
			{ this->FromString( "ID" ),  },
		};
 		productions.push_back( Production( "Type", tvec, 25 ) );
		tvec.clear();
	// Production: 26
		tvec = {
			{ this->FromString( "boolean" ),  },
			{ this->FromString( "int" ),  },
		};
 		productions.push_back( Production( "BuiltInType", tvec, 27 ) );
		tvec.clear();
	// Production: 28
		tvec = {
			{ this->FromString( "BuiltInType" ), this->FromString( "ID" ), this->FromString( "=" ), this->FromString( "Expr" ), this->FromString( ";" ),  },
			{ this->FromString( "ID" ), this->FromString( "StmtRHS" ), this->FromString( ";" ),  },
			{ this->FromString( "{" ), this->FromString( "StmtLst" ), this->FromString( "}" ),  },
			{ this->FromString( "if" ), this->FromString( "(" ), this->FromString( "Expr" ), this->FromString( ")" ), this->FromString( "Stmt" ), this->FromString( "else" ), this->FromString( "Stmt" ),  },
			{ this->FromString( "while" ), this->FromString( "(" ), this->FromString( "Expr" ), this->FromString( ")" ), this->FromString( "Stmt" ),  },
			{ this->FromString( "System.out.println" ), this->FromString( "(" ), this->FromString( "Expr" ), this->FromString( ")" ), this->FromString( ";" ),  },
		};
 		productions.push_back( Production( "Stmt", tvec, 33 ) );
		tvec.clear();
	// Production: 34
		tvec = {
			{ this->FromString( "=" ), this->FromString( "Expr" ),  },
			{ this->FromString( "ID" ), this->FromString( "=" ), this->FromString( "Expr" ),  },
		};
 		productions.push_back( Production( "StmtRHS", tvec, 35 ) );
		tvec.clear();
	// Production: 36
		tvec = {
			{ this->FromString( "Stmt" ), this->FromString( "StmtLst" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "StmtLst", tvec, 37 ) );
		tvec.clear();
	// Production: 38
		tvec = {
			{ this->FromString( "BoolExpr" ),  },
		};
 		productions.push_back( Production( "Expr", tvec, 38 ) );
		tvec.clear();
	// Production: 39
		tvec = {
			{ this->FromString( "CompExpr" ), this->FromString( "BoolExpr_" ),  },
		};
 		productions.push_back( Production( "BoolExpr", tvec, 39 ) );
		tvec.clear();
	// Production: 40
		tvec = {
			{ this->FromString( "&&" ), this->FromString( "CompExpr" ), this->FromString( "BoolExpr_" ),  },
			{ this->FromString( "||" ), this->FromString( "CompExpr" ), this->FromString( "BoolExpr_" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "BoolExpr_", tvec, 42 ) );
		tvec.clear();
	// Production: 43
		tvec = {
			{ this->FromString( "AddExpr" ), this->FromString( "CompExpr_" ),  },
		};
 		productions.push_back( Production( "CompExpr", tvec, 43 ) );
		tvec.clear();
	// Production: 44
		tvec = {
			{ this->FromString( ">" ), this->FromString( "AddExpr" ),  },
			{ this->FromString( ">=" ), this->FromString( "AddExpr" ),  },
			{ this->FromString( "<=" ), this->FromString( "AddExpr" ),  },
			{ this->FromString( "<" ), this->FromString( "AddExpr" ),  },
			{ this->FromString( "==" ), this->FromString( "AddExpr" ),  },
			{ this->FromString( "!=" ), this->FromString( "AddExpr" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "CompExpr_", tvec, 50 ) );
		tvec.clear();
	// Production: 51
		tvec = {
			{ this->FromString( "MultExpr" ), this->FromString( "AddExpr_" ),  },
		};
 		productions.push_back( Production( "AddExpr", tvec, 51 ) );
		tvec.clear();
	// Production: 52
		tvec = {
			{ this->FromString( "+" ), this->FromString( "MultExpr" ), this->FromString( "AddExpr_" ),  },
			{ this->FromString( "-" ), this->FromString( "MultExpr" ), this->FromString( "AddExpr_" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "AddExpr_", tvec, 54 ) );
		tvec.clear();
	// Production: 55
		tvec = {
			{ this->FromString( "NegExpr" ), this->FromString( "MultExpr_" ),  },
		};
 		productions.push_back( Production( "MultExpr", tvec, 55 ) );
		tvec.clear();
	// Production: 56
		tvec = {
			{ this->FromString( "*" ), this->FromString( "NegExpr" ), this->FromString( "MultExpr_" ),  },
			{ this->FromString( "/" ), this->FromString( "NegExpr" ), this->FromString( "MultExpr_" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "MultExpr_", tvec, 58 ) );
		tvec.clear();
	// Production: 59
		tvec = {
			{ this->FromString( "-" ), this->FromString( "Constant" ),  },
			{ this->FromString( "!" ), this->FromString( "Constant" ),  },
			{ this->FromString( "Constant" ),  },
		};
 		productions.push_back( Production( "NegExpr", tvec, 61 ) );
		tvec.clear();
	// Production: 62
		tvec = {
			{ this->FromString( "Literal" ), this->FromString( "DotExpr" ),  },
		};
 		productions.push_back( Production( "Constant", tvec, 62 ) );
		tvec.clear();
	// Production: 63
		tvec = {
			{ this->FromString( "ID" ),  },
			{ this->FromString( "this" ),  },
			{ this->FromString( "Integer" ),  },
			{ this->FromString( "null" ),  },
			{ this->FromString( "true" ),  },
			{ this->FromString( "false" ),  },
			{ this->FromString( "(" ), this->FromString( "Expr" ), this->FromString( ")" ),  },
			{ this->FromString( "new" ), this->FromString( "ID" ), this->FromString( "(" ), this->FromString( ")" ),  },
		};
 		productions.push_back( Production( "Literal", tvec, 70 ) );
		tvec.clear();
	// Production: 71
		tvec = {
			{ this->FromString( "." ), this->FromString( "ID" ), this->FromString( "(" ), this->FromString( "ExprLst" ), this->FromString( ")" ), this->FromString( "DotExpr" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "DotExpr", tvec, 72 ) );
		tvec.clear();
	// Production: 73
		tvec = {
			{ this->FromString( "Expr" ), this->FromString( "ExprLst_" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "ExprLst", tvec, 74 ) );
		tvec.clear();
	// Production: 75
		tvec = {
			{ this->FromString( "," ), this->FromString( "ExprLst" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "ExprLst_", tvec, 76 ) );
		tvec.clear();


        // Table creation goes here.
	// Table: 0
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 0;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 77;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 1
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 1;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 77;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 2
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 2;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 77;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 3
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 3;
		tmap["."] = 78;
		tmap["extends"] = 77;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 77;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 4
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 77;
		tmap["."] = 78;
		tmap["extends"] = 4;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 77;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 5;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 5
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 6;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 6;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 7;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 6;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 8;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 6
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 9;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 77;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 7
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 8
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 11;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 12;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 9
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 13;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 13;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 77;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 13;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 77;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 10
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 14;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 14;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 15;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 14;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 15;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 11
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 16;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 77;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 12
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 17;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 18;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 13
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 19;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 19;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 20;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 19;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 14
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 22;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 21;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 15
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 23;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 23;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 77;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 77;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 23;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 16
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 24;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 24;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 25;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 17
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 27;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 26;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 77;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 18
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 28;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 33;
		tmap["boolean"] = 28;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 31;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 77;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 77;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 29;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 32;
		tmap["{"] = 30;
		tmap["}"] = 77;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 19
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 77;
		tmap["main"] = 78;
		tmap["="] = 34;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 35;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 20
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 36;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 36;
		tmap["boolean"] = 36;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 36;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 78;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 37;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 36;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 36;
		tmap["{"] = 36;
		tmap["}"] = 37;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 21
		tmap["false"] = 38;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 38;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 38;
		tmap["String"] = 78;
		tmap[")"] = 77;
		tmap["("] = 38;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 38;
		tmap[","] = 77;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 38;
		tmap[";"] = 77;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 38;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 38;
		tmap["$"] = 78;
		tmap["this"] = 38;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 38;
		table.push_back( tmap );
		tmap.clear();
	// Table: 22
		tmap["false"] = 39;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 39;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 39;
		tmap["String"] = 78;
		tmap[")"] = 77;
		tmap["("] = 39;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 39;
		tmap[","] = 77;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 39;
		tmap[";"] = 77;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 39;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 39;
		tmap["$"] = 78;
		tmap["this"] = 39;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 39;
		table.push_back( tmap );
		tmap.clear();
	// Table: 23
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 41;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 42;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 42;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 42;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 40;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 24
		tmap["false"] = 43;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 77;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 43;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 43;
		tmap["String"] = 78;
		tmap[")"] = 77;
		tmap["("] = 43;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 43;
		tmap[","] = 77;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 43;
		tmap[";"] = 77;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 77;
		tmap["new"] = 43;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 43;
		tmap["$"] = 78;
		tmap["this"] = 43;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 43;
		table.push_back( tmap );
		tmap.clear();
	// Table: 25
		tmap["false"] = 78;
		tmap[">="] = 45;
		tmap["int"] = 78;
		tmap["||"] = 50;
		tmap["<="] = 46;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 49;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 50;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 50;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 50;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 44;
		tmap["return"] = 78;
		tmap["=="] = 48;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 50;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 47;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 26
		tmap["false"] = 51;
		tmap[">="] = 77;
		tmap["int"] = 78;
		tmap["||"] = 77;
		tmap["<="] = 77;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 51;
		tmap["!="] = 77;
		tmap["if"] = 78;
		tmap["!"] = 51;
		tmap["String"] = 78;
		tmap[")"] = 77;
		tmap["("] = 51;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 51;
		tmap[","] = 77;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 51;
		tmap[";"] = 77;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 77;
		tmap["return"] = 78;
		tmap["=="] = 77;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 77;
		tmap["new"] = 51;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 51;
		tmap["$"] = 78;
		tmap["this"] = 51;
		tmap["<"] = 77;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 51;
		table.push_back( tmap );
		tmap.clear();
	// Table: 27
		tmap["false"] = 78;
		tmap[">="] = 54;
		tmap["int"] = 78;
		tmap["||"] = 54;
		tmap["<="] = 54;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 54;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 54;
		tmap["("] = 78;
		tmap["+"] = 52;
		tmap["*"] = 78;
		tmap["-"] = 53;
		tmap[","] = 54;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 54;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 54;
		tmap["return"] = 78;
		tmap["=="] = 54;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 54;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 54;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 28
		tmap["false"] = 55;
		tmap[">="] = 77;
		tmap["int"] = 78;
		tmap["||"] = 77;
		tmap["<="] = 77;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 55;
		tmap["!="] = 77;
		tmap["if"] = 78;
		tmap["!"] = 55;
		tmap["String"] = 78;
		tmap[")"] = 77;
		tmap["("] = 55;
		tmap["+"] = 77;
		tmap["*"] = 78;
		tmap["-"] = 55;
		tmap[","] = 77;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 55;
		tmap[";"] = 77;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 77;
		tmap["return"] = 78;
		tmap["=="] = 77;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 77;
		tmap["new"] = 55;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 55;
		tmap["$"] = 78;
		tmap["this"] = 55;
		tmap["<"] = 77;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 55;
		table.push_back( tmap );
		tmap.clear();
	// Table: 29
		tmap["false"] = 78;
		tmap[">="] = 58;
		tmap["int"] = 78;
		tmap["||"] = 58;
		tmap["<="] = 58;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 58;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 58;
		tmap["("] = 78;
		tmap["+"] = 58;
		tmap["*"] = 56;
		tmap["-"] = 58;
		tmap[","] = 58;
		tmap["/"] = 57;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 58;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 58;
		tmap["return"] = 78;
		tmap["=="] = 58;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 58;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 58;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 30
		tmap["false"] = 61;
		tmap[">="] = 77;
		tmap["int"] = 78;
		tmap["||"] = 77;
		tmap["<="] = 77;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 61;
		tmap["!="] = 77;
		tmap["if"] = 78;
		tmap["!"] = 60;
		tmap["String"] = 78;
		tmap[")"] = 77;
		tmap["("] = 61;
		tmap["+"] = 77;
		tmap["*"] = 77;
		tmap["-"] = 59;
		tmap[","] = 77;
		tmap["/"] = 77;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 61;
		tmap[";"] = 77;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 77;
		tmap["return"] = 78;
		tmap["=="] = 77;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 77;
		tmap["new"] = 61;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 61;
		tmap["$"] = 78;
		tmap["this"] = 61;
		tmap["<"] = 77;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 61;
		table.push_back( tmap );
		tmap.clear();
	// Table: 31
		tmap["false"] = 62;
		tmap[">="] = 77;
		tmap["int"] = 78;
		tmap["||"] = 77;
		tmap["<="] = 77;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 62;
		tmap["!="] = 77;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 77;
		tmap["("] = 62;
		tmap["+"] = 77;
		tmap["*"] = 77;
		tmap["-"] = 77;
		tmap[","] = 77;
		tmap["/"] = 77;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 62;
		tmap[";"] = 77;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 77;
		tmap["return"] = 78;
		tmap["=="] = 77;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 77;
		tmap["new"] = 62;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 62;
		tmap["$"] = 78;
		tmap["this"] = 62;
		tmap["<"] = 77;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 62;
		table.push_back( tmap );
		tmap.clear();
	// Table: 32
		tmap["false"] = 68;
		tmap[">="] = 77;
		tmap["int"] = 78;
		tmap["||"] = 77;
		tmap["<="] = 77;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 66;
		tmap["!="] = 77;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 77;
		tmap["("] = 69;
		tmap["+"] = 77;
		tmap["*"] = 77;
		tmap["-"] = 77;
		tmap[","] = 77;
		tmap["/"] = 77;
		tmap["class"] = 78;
		tmap["."] = 77;
		tmap["extends"] = 78;
		tmap["Integer"] = 65;
		tmap[";"] = 77;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 77;
		tmap["return"] = 78;
		tmap["=="] = 77;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 77;
		tmap["new"] = 70;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 63;
		tmap["$"] = 78;
		tmap["this"] = 64;
		tmap["<"] = 77;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 67;
		table.push_back( tmap );
		tmap.clear();
	// Table: 33
		tmap["false"] = 78;
		tmap["int"] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap["("] = 78;
		tmap["class"] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap["return"] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();
	// Table: 34
		tmap["false"] = 73;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 73;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 73;
		tmap["String"] = 78;
		tmap[")"] = 74;
		tmap["("] = 73;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 73;
		tmap[","] = 78;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 73;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 73;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 73;
		tmap["$"] = 78;
		tmap["this"] = 73;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 73;
		table.push_back( tmap );
		tmap.clear();
	// Table: 35
		tmap["false"] = 78;
		tmap[">="] = 78;
		tmap["int"] = 78;
		tmap["||"] = 78;
		tmap["<="] = 78;
		tmap["System.out.println"] = 78;
		tmap["boolean"] = 78;
		tmap["static"] = 78;
		tmap["null"] = 78;
		tmap["!="] = 78;
		tmap["if"] = 78;
		tmap["!"] = 78;
		tmap["String"] = 78;
		tmap[")"] = 76;
		tmap["("] = 78;
		tmap["+"] = 78;
		tmap["*"] = 78;
		tmap["-"] = 78;
		tmap[","] = 75;
		tmap["/"] = 78;
		tmap["class"] = 78;
		tmap["."] = 78;
		tmap["extends"] = 78;
		tmap["Integer"] = 78;
		tmap[";"] = 78;
		tmap["main"] = 78;
		tmap["="] = 78;
		tmap["public"] = 78;
		tmap[">"] = 78;
		tmap["return"] = 78;
		tmap["=="] = 78;
		tmap["void"] = 78;
		tmap["else"] = 78;
		tmap["&&"] = 78;
		tmap["new"] = 78;
		tmap["["] = 78;
		tmap["]"] = 78;
		tmap["ID"] = 78;
		tmap["$"] = 78;
		tmap["this"] = 78;
		tmap["<"] = 78;
		tmap["while"] = 78;
		tmap["{"] = 78;
		tmap["}"] = 78;
		tmap["true"] = 78;
		table.push_back( tmap );
		tmap.clear();


        // Frist set stuff
		firstset["ClassHeader"] = { "class", };
		firstset["ClassDeclRHS"] = { "extends", "{", };
		firstset["ClassBody"] = { "EPSILON", "ID", "boolean", "int", "public", };
		firstset["MaybeMehtods"] = { "EPSILON", };
		firstset["ClassDecls"] = { "EPSILON", "class", };
		firstset["ClassVarDecls"] = { "EPSILON", "ID", "boolean", "int", };
		firstset["MethodDecl"] = { "public", };
		firstset["MethodDecls"] = { "EPSILON", "public", };
		firstset["MethodParams"] = { "EPSILON", "ID", "boolean", "int", };
		firstset["MethodParams_"] = { "COM", "EPSILON", };
		firstset["Type"] = { "ID", "boolean", "int", };
		firstset["BuiltInType"] = { "boolean", "int", };
		firstset["Stmt"] = { "ID", "{", "if", "while", "System.out.println", "boolean", "int", };
		firstset["StmtRHS"] = { "=", "ID", };
		firstset["StmtLst"] = { "EPSILON", "ID", "{", "if", "while", "System.out.println", "boolean", "int", };
		firstset["BoolExpr_"] = { "||", "@@", "EPSILON", };
		firstset["CompExpr_"] = { ">", ">=", "<=", "<", "==", "!=", "EPSILON", };
		firstset["AddExpr_"] = { "+", "-", "EPSILON", };
		firstset["MultExpr_"] = { "*", "/", "EPSILON", };
		firstset["NegExpr"] = { "-", "!", "ID", "this", "Integer", "null", "true", "false", "(", "new", };
		firstset["Literal"] = { "ID", "this", "Integer", "null", "true", "false", "(", "new", };
		firstset["DotExpr"] = { ".", "EPSILON", };
		firstset["ExprLst"] = { "EPSILON", "-", "!", "ID", "this", "Integer", "null", "true", "false", "(", "new", };
		firstset["ExprLst_"] = { "COM", "EPSILON", };
		firstset["MainClassDecl"] = { "class", };
		firstset["ClassDecl"] = { "class", };
		firstset["Constant"] = { "ID", "this", "Integer", "null", "true", "false", "(", "new", };
		firstset["Program"] = { "class", };
		firstset["MaybeMethods"] = { "EPSILON", "public", };
		firstset["ClassVarDecl"] = { "ID", "boolean", "int", };
		firstset["Formal"] = { "ID", "boolean", "int", };
		firstset["MultExpr"] = { "-", "!", "ID", "this", "Integer", "null", "true", "false", "(", "new", };
		firstset["AddExpr"] = { "-", "!", "ID", "this", "Integer", "null", "true", "false", "(", "new", };
		firstset["CompExpr"] = { "-", "!", "ID", "this", "Integer", "null", "true", "false", "(", "new", };
		firstset["BoolExpr"] = { "-", "!", "ID", "this", "Integer", "null", "true", "false", "(", "new", };
		firstset["Expr"] = { "-", "!", "ID", "this", "Integer", "null", "true", "false", "(", "new", };

    }

    bool is_production( const string &str )
    {
        for( string p : prod_names )
            if( p.compare( str ) == 0 )
                return true;
        return false;
    }

    const Production &getProductionByGroup( int i ){
        return productions.at( i );
    }

    int prodIdToGroupId( int i )
    {
        int index = 0;
        int group_ID = 0;
        for( Production p : productions )
        {
            for( vector<BasicToken*> v : p.rhs )
            {
                if( index == i )
                    return group_ID;
                ++index;
            }
        }
        return -1;
    }

    int prodGroupId( const string &name )
    {
        int group_ID = 0;
        for( Production p : productions )
        {
            if( p.lhs.compare( name ) == 0 )
                return group_ID;
            ++group_ID;
        }
        return -1;
    }

    vector<BasicToken*> getRHSById( int i ){
        int index = 0;
        for( Production p : productions )
        {
            for( vector<BasicToken*> v : p.rhs )
            {
                if( index == i )
                    return v;
                ++index;
            }
        }

        throw "No Such Element";
    }

    /**
        Return:
            >= 0 : Index of the next Production's ID into the current Production
            -1   : The non-terminal is not refrenced from the current Production
                   It may be ID, Integer, or an Error.

            TODO: Fix this.
    */
    int getNextProdIndex( int curProdGroup, const string &nonTerminal ) {
        if( table[curProdGroup].count( nonTerminal ) == 0 )
            return -1;
        else
            return table[curProdGroup][nonTerminal];
    }

    vector<string> first( const string &name ) {
        return firstset[name];
    }

private:
    BasicToken* FromString( const string &str )
    {
        // First, check if it's the name of a NonTerminal
        if( str.compare( "EPSILON" ) == 0 )
        {
            return new Epsilon();
        }
        if( str.compare( "ID" ) == 0 )
        {
            return new Identifier( 0, 0, Identifiers::Valid_Identifier, "" );
        }
        if( str.compare( "$" ) == 0 )
        {
            return new EndOfFileToken();
        }
        else if( str.compare( "Integer" ) == 0 )
        {
            return new Number( 0, 0, Numbers::Valid_Number, 0 );
        }
        else if( is_production( str ) )
        {
            return new NonTerminal( str );
        }
        else
        {
            BasicToken *tok = LexerTokenFactory::FromString( str );
            if( tok == nullptr )
                cout << "Unable to parse Token: " << str << endl;
            return tok;
        }
    }
};

#endif

