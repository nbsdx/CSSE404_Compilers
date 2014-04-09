
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
		prod_names.push_back( "DotExpr" );
		prod_names.push_back( "DotExpr_" );
		prod_names.push_back( "Literal" );
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
			{ this->FromString( "extends" ), this->FromString( "ID" ), this->FromString( "{" ), this->FromString( "ClassBody" ), this->FromString( "}" ),  },
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
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "MaybeMethods", tvec, 10 ) );
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
			{ this->FromString( "DotExpr_" ),  },
		};
 		productions.push_back( Production( "StmtRHS", tvec, 36 ) );
		tvec.clear();
	// Production: 37
		tvec = {
			{ this->FromString( "Stmt" ), this->FromString( "StmtLst" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "StmtLst", tvec, 38 ) );
		tvec.clear();
	// Production: 39
		tvec = {
			{ this->FromString( "BoolExpr" ),  },
		};
 		productions.push_back( Production( "Expr", tvec, 39 ) );
		tvec.clear();
	// Production: 40
		tvec = {
			{ this->FromString( "CompExpr" ), this->FromString( "BoolExpr_" ),  },
		};
 		productions.push_back( Production( "BoolExpr", tvec, 40 ) );
		tvec.clear();
	// Production: 41
		tvec = {
			{ this->FromString( "&&" ), this->FromString( "CompExpr" ), this->FromString( "BoolExpr_" ),  },
			{ this->FromString( "||" ), this->FromString( "CompExpr" ), this->FromString( "BoolExpr_" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "BoolExpr_", tvec, 43 ) );
		tvec.clear();
	// Production: 44
		tvec = {
			{ this->FromString( "AddExpr" ), this->FromString( "CompExpr_" ),  },
		};
 		productions.push_back( Production( "CompExpr", tvec, 44 ) );
		tvec.clear();
	// Production: 45
		tvec = {
			{ this->FromString( ">" ), this->FromString( "AddExpr" ),  },
			{ this->FromString( ">=" ), this->FromString( "AddExpr" ),  },
			{ this->FromString( "<=" ), this->FromString( "AddExpr" ),  },
			{ this->FromString( "<" ), this->FromString( "AddExpr" ),  },
			{ this->FromString( "==" ), this->FromString( "AddExpr" ),  },
			{ this->FromString( "!=" ), this->FromString( "AddExpr" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "CompExpr_", tvec, 51 ) );
		tvec.clear();
	// Production: 52
		tvec = {
			{ this->FromString( "MultExpr" ), this->FromString( "AddExpr_" ),  },
		};
 		productions.push_back( Production( "AddExpr", tvec, 52 ) );
		tvec.clear();
	// Production: 53
		tvec = {
			{ this->FromString( "+" ), this->FromString( "MultExpr" ), this->FromString( "AddExpr_" ),  },
			{ this->FromString( "-" ), this->FromString( "MultExpr" ), this->FromString( "AddExpr_" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "AddExpr_", tvec, 55 ) );
		tvec.clear();
	// Production: 56
		tvec = {
			{ this->FromString( "NegExpr" ), this->FromString( "MultExpr_" ),  },
		};
 		productions.push_back( Production( "MultExpr", tvec, 56 ) );
		tvec.clear();
	// Production: 57
		tvec = {
			{ this->FromString( "*" ), this->FromString( "NegExpr" ), this->FromString( "MultExpr_" ),  },
			{ this->FromString( "/" ), this->FromString( "NegExpr" ), this->FromString( "MultExpr_" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "MultExpr_", tvec, 59 ) );
		tvec.clear();
	// Production: 60
		tvec = {
			{ this->FromString( "-" ), this->FromString( "DotExpr" ),  },
			{ this->FromString( "!" ), this->FromString( "DotExpr" ),  },
			{ this->FromString( "DotExpr" ),  },
		};
 		productions.push_back( Production( "NegExpr", tvec, 62 ) );
		tvec.clear();
	// Production: 63
		tvec = {
			{ this->FromString( "Literal" ), this->FromString( "DotExpr_" ),  },
		};
 		productions.push_back( Production( "DotExpr", tvec, 63 ) );
		tvec.clear();
	// Production: 64
		tvec = {
			{ this->FromString( "EPSILON" ),  },
			{ this->FromString( "." ), this->FromString( "ID" ), this->FromString( "(" ), this->FromString( "ExprLst" ), this->FromString( ")" ), this->FromString( "DotExpr_" ),  },
		};
 		productions.push_back( Production( "DotExpr_", tvec, 65 ) );
		tvec.clear();
	// Production: 66
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
 		productions.push_back( Production( "Literal", tvec, 73 ) );
		tvec.clear();
	// Production: 74
		tvec = {
			{ this->FromString( "Expr" ), this->FromString( "ExprLst_" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "ExprLst", tvec, 75 ) );
		tvec.clear();
	// Production: 76
		tvec = {
			{ this->FromString( "," ), this->FromString( "ExprLst" ),  },
			{ this->FromString( "EPSILON" ),  },
		};
 		productions.push_back( Production( "ExprLst_", tvec, 77 ) );
		tvec.clear();


        // Table creation goes here.
	// Table: 0
		tmap["class"] = 0;
		table.push_back( tmap );
		tmap.clear();
	// Table: 1
		tmap["class"] = 1;
		table.push_back( tmap );
		tmap.clear();
	// Table: 2
		tmap["class"] = 2;
		table.push_back( tmap );
		tmap.clear();
	// Table: 3
		tmap["class"] = 3;
		table.push_back( tmap );
		tmap.clear();
	// Table: 4
		tmap["{"] = 5;
		tmap["extends"] = 4;
		table.push_back( tmap );
		tmap.clear();
	// Table: 5
		tmap["int"] = 6;
		tmap["boolean"] = 6;
		tmap["}"] = 8;
		tmap["public"] = 7;
		tmap["ID"] = 6;
		table.push_back( tmap );
		tmap.clear();
	// Table: 6
		tmap["}"] = 10;
		tmap["public"] = 9;
		table.push_back( tmap );
		tmap.clear();
	// Table: 7
		tmap["class"] = 11;
		tmap["$"] = 12;
		table.push_back( tmap );
		tmap.clear();
	// Table: 8
		tmap["int"] = 13;
		tmap["boolean"] = 13;
		tmap["ID"] = 13;
		table.push_back( tmap );
		tmap.clear();
	// Table: 9
		tmap["int"] = 14;
		tmap["boolean"] = 14;
		tmap["}"] = 15;
		tmap["public"] = 15;
		tmap["ID"] = 14;
		table.push_back( tmap );
		tmap.clear();
	// Table: 10
		tmap["public"] = 16;
		table.push_back( tmap );
		tmap.clear();
	// Table: 11
		tmap["}"] = 18;
		tmap["public"] = 17;
		table.push_back( tmap );
		tmap.clear();
	// Table: 12
		tmap[")"] = 20;
		tmap["int"] = 19;
		tmap["boolean"] = 19;
		tmap["ID"] = 19;
		table.push_back( tmap );
		tmap.clear();
	// Table: 13
		tmap[")"] = 22;
		tmap[","] = 21;
		table.push_back( tmap );
		tmap.clear();
	// Table: 14
		tmap["int"] = 23;
		tmap["boolean"] = 23;
		tmap["ID"] = 23;
		table.push_back( tmap );
		tmap.clear();
	// Table: 15
		tmap["int"] = 24;
		tmap["boolean"] = 24;
		tmap["ID"] = 25;
		table.push_back( tmap );
		tmap.clear();
	// Table: 16
		tmap["int"] = 27;
		tmap["boolean"] = 26;
		table.push_back( tmap );
		tmap.clear();
	// Table: 17
		tmap["int"] = 28;
		tmap["System.out.println"] = 33;
		tmap["while"] = 32;
		tmap["boolean"] = 28;
		tmap["{"] = 30;
		tmap["ID"] = 29;
		tmap["if"] = 31;
		table.push_back( tmap );
		tmap.clear();
	// Table: 18
		tmap["="] = 34;
		tmap["ID"] = 35;
		tmap["."] = 36;
		table.push_back( tmap );
		tmap.clear();
	// Table: 19
		tmap["return"] = 38;
		tmap["int"] = 37;
		tmap["System.out.println"] = 37;
		tmap["while"] = 37;
		tmap["boolean"] = 37;
		tmap["{"] = 37;
		tmap["}"] = 38;
		tmap["ID"] = 37;
		tmap["if"] = 37;
		table.push_back( tmap );
		tmap.clear();
	// Table: 20
		tmap["!"] = 39;
		tmap["false"] = 39;
		tmap["this"] = 39;
		tmap["("] = 39;
		tmap["-"] = 39;
		tmap["new"] = 39;
		tmap["Integer"] = 39;
		tmap["null"] = 39;
		tmap["true"] = 39;
		tmap["ID"] = 39;
		table.push_back( tmap );
		tmap.clear();
	// Table: 21
		tmap["!"] = 40;
		tmap["false"] = 40;
		tmap["this"] = 40;
		tmap["("] = 40;
		tmap["-"] = 40;
		tmap["new"] = 40;
		tmap["Integer"] = 40;
		tmap["null"] = 40;
		tmap["true"] = 40;
		tmap["ID"] = 40;
		table.push_back( tmap );
		tmap.clear();
	// Table: 22
		tmap[")"] = 43;
		tmap[";"] = 43;
		tmap["@@"] = 42;
		tmap[","] = 43;
		tmap["&&"] = 41;
		table.push_back( tmap );
		tmap.clear();
	// Table: 23
		tmap["!"] = 44;
		tmap["false"] = 44;
		tmap["this"] = 44;
		tmap["("] = 44;
		tmap["-"] = 44;
		tmap["new"] = 44;
		tmap["Integer"] = 44;
		tmap["null"] = 44;
		tmap["true"] = 44;
		tmap["ID"] = 44;
		table.push_back( tmap );
		tmap.clear();
	// Table: 24
		tmap["@@"] = 51;
		tmap[">="] = 46;
		tmap[")"] = 51;
		tmap["<="] = 47;
		tmap[","] = 51;
		tmap["=="] = 49;
		tmap["&&"] = 51;
		tmap[";"] = 51;
		tmap["!="] = 50;
		tmap["<"] = 48;
		tmap[">"] = 45;
		table.push_back( tmap );
		tmap.clear();
	// Table: 25
		tmap["!"] = 52;
		tmap["false"] = 52;
		tmap["this"] = 52;
		tmap["("] = 52;
		tmap["-"] = 52;
		tmap["new"] = 52;
		tmap["Integer"] = 52;
		tmap["null"] = 52;
		tmap["true"] = 52;
		tmap["ID"] = 52;
		table.push_back( tmap );
		tmap.clear();
	// Table: 26
		tmap["@@"] = 55;
		tmap[">="] = 55;
		tmap[")"] = 55;
		tmap["+"] = 53;
		tmap["<="] = 55;
		tmap["-"] = 54;
		tmap[","] = 55;
		tmap["=="] = 55;
		tmap["&&"] = 55;
		tmap[";"] = 55;
		tmap["!="] = 55;
		tmap["<"] = 55;
		tmap[">"] = 55;
		table.push_back( tmap );
		tmap.clear();
	// Table: 27
		tmap["!"] = 56;
		tmap["false"] = 56;
		tmap["this"] = 56;
		tmap["("] = 56;
		tmap["-"] = 56;
		tmap["new"] = 56;
		tmap["Integer"] = 56;
		tmap["null"] = 56;
		tmap["true"] = 56;
		tmap["ID"] = 56;
		table.push_back( tmap );
		tmap.clear();
	// Table: 28
		tmap["@@"] = 59;
		tmap["*"] = 57;
		tmap[">="] = 59;
		tmap[")"] = 59;
		tmap["+"] = 59;
		tmap["<="] = 59;
		tmap["-"] = 59;
		tmap[","] = 59;
		tmap["/"] = 58;
		tmap["=="] = 59;
		tmap["&&"] = 59;
		tmap[";"] = 59;
		tmap["!="] = 59;
		tmap["<"] = 59;
		tmap[">"] = 59;
		table.push_back( tmap );
		tmap.clear();
	// Table: 29
		tmap["!"] = 61;
		tmap["false"] = 62;
		tmap["this"] = 62;
		tmap["("] = 62;
		tmap["-"] = 60;
		tmap["new"] = 62;
		tmap["Integer"] = 62;
		tmap["null"] = 62;
		tmap["true"] = 62;
		tmap["ID"] = 62;
		table.push_back( tmap );
		tmap.clear();
	// Table: 30
		tmap["false"] = 63;
		tmap["this"] = 63;
		tmap["("] = 63;
		tmap["new"] = 63;
		tmap["Integer"] = 63;
		tmap["null"] = 63;
		tmap["true"] = 63;
		tmap["ID"] = 63;
		table.push_back( tmap );
		tmap.clear();
	// Table: 31
		tmap["@@"] = 64;
		tmap["*"] = 64;
		tmap[">="] = 64;
		tmap[")"] = 64;
		tmap["+"] = 64;
		tmap["<="] = 64;
		tmap["-"] = 64;
		tmap[","] = 64;
		tmap["/"] = 64;
		tmap["."] = 65;
		tmap["=="] = 64;
		tmap["&&"] = 64;
		tmap[";"] = 64;
		tmap["!="] = 64;
		tmap["<"] = 64;
		tmap[">"] = 64;
		table.push_back( tmap );
		tmap.clear();
	// Table: 32
		tmap["false"] = 71;
		tmap["this"] = 67;
		tmap["("] = 72;
		tmap["new"] = 73;
		tmap["Integer"] = 68;
		tmap["null"] = 69;
		tmap["true"] = 70;
		tmap["ID"] = 66;
		table.push_back( tmap );
		tmap.clear();
	// Table: 33
		tmap["!"] = 74;
		tmap["false"] = 74;
		tmap[")"] = 75;
		tmap["("] = 74;
		tmap["-"] = 74;
		tmap["this"] = 74;
		tmap["new"] = 74;
		tmap["Integer"] = 74;
		tmap["null"] = 74;
		tmap["true"] = 74;
		tmap["ID"] = 74;
		table.push_back( tmap );
		tmap.clear();
	// Table: 34
		tmap[")"] = 77;
		tmap[","] = 76;
		table.push_back( tmap );
		tmap.clear();


        // Frist set stuff
		firstset["ClassHeader"] = { "class", };
		firstset["ClassDeclRHS"] = { "extends", "{", };
		firstset["ClassBody"] = { "EPSILON", "ID", "boolean", "int", "public", };
		firstset["MaybeMethods"] = { "EPSILON", "public", };
		firstset["ClassDecls"] = { "EPSILON", "class", };
		firstset["ClassVarDecls"] = { "EPSILON", "ID", "boolean", "int", };
		firstset["MethodDecl"] = { "public", };
		firstset["MethodDecls"] = { "EPSILON", "public", };
		firstset["MethodParams"] = { "EPSILON", "ID", "boolean", "int", };
		firstset["MethodParams_"] = { ",", "EPSILON", };
		firstset["Type"] = { "ID", "boolean", "int", };
		firstset["BuiltInType"] = { "boolean", "int", };
		firstset["Stmt"] = { "ID", "{", "if", "while", "System.out.println", "boolean", "int", };
		firstset["StmtRHS"] = { "=", "ID", "EPSILON", ".", };
		firstset["StmtLst"] = { "EPSILON", "ID", "{", "if", "while", "System.out.println", "boolean", "int", };
		firstset["BoolExpr_"] = { "&&", "@@", "EPSILON", };
		firstset["CompExpr_"] = { ">", ">=", "<=", "<", "==", "!=", "EPSILON", };
		firstset["AddExpr_"] = { "+", "-", "EPSILON", };
		firstset["MultExpr_"] = { "*", "/", "EPSILON", };
		firstset["NegExpr"] = { "-", "!", "ID", "this", "Integer", "null", "true", "false", "(", "new", };
		firstset["DotExpr_"] = { "EPSILON", ".", };
		firstset["Literal"] = { "ID", "this", "Integer", "null", "true", "false", "(", "new", };
		firstset["ExprLst"] = { "EPSILON", "-", "!", "ID", "this", "Integer", "null", "true", "false", "(", "new", };
		firstset["ExprLst_"] = { ",", "EPSILON", };
		firstset["MainClassDecl"] = { "class", };
		firstset["ClassDecl"] = { "class", };
		firstset["DotExpr"] = { "ID", "this", "Integer", "null", "true", "false", "(", "new", };
		firstset["Program"] = { "class", };
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

