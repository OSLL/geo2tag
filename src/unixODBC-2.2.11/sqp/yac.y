%{
#include <string.h>
#include <math.h>


#include "sqp.h"

#define YYDEBUG 1

%}
	
/* intrinsic types */
%union {
	int intval;
	double floatval;
	char *strval;
	int subtok;
	HSQPCOMPARISON hComp;
	HSQPCOND hCond;
}

/* variable types */
%token <strval> NAME
%token <strval> STRING
%token <intval> INTNUM 
%token <floatval> APPROXNUM

/* keywords */
%token SELECT FROM WHERE
%token DELETE
%token INSERT INTO VALUES
%token UPDATE SET
%token CURRENT OF
%token CREATE DROP TABLE
%token ORDER BY
%token ASC DESC
%token CHAR VARCHAR LONG NUMERIC DECIMAL SMALLINT INTEGER REAL FLOAT DOUBLE PRECISION BIT TINYINT BIGINT BINARY VARBINARY DATE TIME TIMESTAMP
%token NULLX
%token IS
%token LIKE ESCAPE

%left <strval> COMPARISON
%left OR
%left AND
%left NOT

/* types */
%type <hComp> y_comparison
%type <hCond> y_comparisons
%type <strval> y_value
%type <strval> y_escape

%%

y_sql:	
		y_createtable
	|	y_droptable
	|	y_select
	|	y_delete
	|	y_insert
	|	y_update
	;
	
y_createtable:
		CREATE TABLE y_table '(' y_columndefs ')'	{ sqpStoreCreateTable(); }
	;

y_droptable:
		DROP TABLE y_table				{ sqpStoreDropTable(); }
	;

y_select:
		SELECT y_columns FROM y_table y_condition y_orderby	{ sqpStoreSelect(); }
	;

y_delete:
		DELETE FROM y_table y_condition			{ sqpStoreDelete(); }
	|	DELETE y_table y_condition			{ sqpStoreDelete(); }
	;

y_insert:
		INSERT INTO y_table y_values			{ sqpStoreInsert(); }
	|	INSERT y_table y_values				{ sqpStoreInsert(); }
	|	INSERT INTO y_table '(' y_column_list ')' y_values			{ sqpStoreInsert(); }
	|	INSERT y_table '(' y_column_list ')' y_values				{ sqpStoreInsert(); }
	;

y_update:
		UPDATE y_table SET y_assignments y_condition	{ sqpStoreUpdate(); }
	;

y_columndefs:
		y_columndef
	|	y_columndefs ',' y_columndef
	;

y_columns:
		y_column_list
	|	'*'						{ sqpStoreColumn( &g_hColumns, "*", 0 ); }
	;
	
y_column_list:
		y_columnname
	|	y_column_list ',' y_columnname
	;

y_table:
		NAME 						{ sqpStoreTable( $1 ); }
	|	NAME NAME					{ sqpStoreTable( $1 ); }
	;
	
y_columndef:
		NAME y_datatype y_columndef_options		{ sqpStoreColumnDef( $1 ); }
	;

y_datatype:
		CHAR '(' INTNUM ')'				{ sqpStoreDataType( "CHAR", $3, 0 ); }
	|	VARCHAR '(' INTNUM ')'				{ sqpStoreDataType( "VARCHAR", $3, 0 ); }
	|	LONG VARCHAR					{ sqpStoreDataType( "LONG VARCHAR", 0, 0 ); }
	|	NUMERIC						{ sqpStoreDataType( "NUMERIC", 0, 0 ); }
	|	NUMERIC '(' INTNUM ')'				{ sqpStoreDataType( "NUMERIC", $3, 0 ); }
	|	NUMERIC '(' INTNUM ',' INTNUM ')'		{ sqpStoreDataType( "NUMERIC", $3, $5 ); }
	|	DECIMAL						{ sqpStoreDataType( "DECIMAL", 0, 0 ); }
	|	DECIMAL '(' INTNUM ')'				{ sqpStoreDataType( "DECIMAL", $3, 0 ); }
	|	DECIMAL '(' INTNUM ',' INTNUM ')'		{ sqpStoreDataType( "DECIMAL", $3, $5 ); }
	|	SMALLINT					{ sqpStoreDataType( "SMALLINT", 0, 0 ); }
	|	INTEGER						{ sqpStoreDataType( "INTEGER", 0, 0 ); }
	|	REAL						{ sqpStoreDataType( "REAL", 0, 0 ); }
	|	FLOAT						{ sqpStoreDataType( "FLOAT", 0, 0 ); }
	|	FLOAT '(' INTNUM ')'				{ sqpStoreDataType( "FLOAT", $3, 0 ); }
	|	DOUBLE PRECISION				{ sqpStoreDataType( "DOUBLE PRECISION", 0, 0 ); }
	|	BIT						{ sqpStoreDataType( "BIT", 0, 0 ); }
	|	TINYINT						{ sqpStoreDataType( "TINYINT", 0, 0 ); }
	|	BIGINT						{ sqpStoreDataType( "BIGINT", 0, 0 ); }
	|	BINARY '(' INTNUM ')'				{ sqpStoreDataType( "BINARY", $3, 0 ); }
	|	VARBINARY '(' INTNUM ')'			{ sqpStoreDataType( "VARBINARY", $3, 0 ); }
	|	LONG VARBINARY					{ sqpStoreDataType( "LONG VARBINARY", 0, 0 ); }
	|	DATE						{ sqpStoreDataType( "DATE", 0, 0 ); }
	|	TIME '(' INTNUM ')'				{ sqpStoreDataType( "TIME", $3, 0 ); }
	|	TIMESTAMP '(' INTNUM ')'			{ sqpStoreDataType( "TIMESTAMP", $3, 0 ); }
	;

y_columndef_options:
		/* optional */
	|	NOT NULLX					{ g_nNulls = 0; }
	;
		
y_columnname:
		NAME						{ sqpStoreColumn( &g_hColumns, $1, 0 ); }
	|	INTNUM						{ sqpStoreColumn( &g_hColumns, 0, $1 ); }
	;

y_condition:
		/* optional */
	|	WHERE y_comparisons					{ g_hConds = $2; }
	|	WHERE CURRENT OF NAME				{ sqpStorePositioned( $4 ); }
	;
	
y_orderby:
		/* optional */
	|	ORDER BY y_orderby_column_list			{ g_nOrderDirection = sqpasc; }
	|	ORDER BY y_orderby_column_list ASC		{ g_nOrderDirection = sqpasc;  }
	|	ORDER BY y_orderby_column_list DESC		{ g_nOrderDirection = sqpdesc; }
	;
	
y_orderby_column_list:
		y_orderby_column
	|	y_orderby_column_list ',' y_orderby_column
	;
	
y_orderby_column:
		NAME						{ sqpStoreColumn( &g_hOrderBy, $1, 0 ); }
	|	INTNUM						{ sqpStoreColumn( &g_hOrderBy, 0, $1 ); }
	;

y_values:
		VALUES '(' y_value_list ')'
	;

y_value_list:
		y_value							{ sqpStoreValue( $1 ); }
	|	y_value_list ',' y_value		{ sqpStoreValue( $3 ); }
	;

y_value:
		STRING						{ $$ = $1;  }
	|	INTNUM						{ char buf[128];
                                      sprintf( buf, "'%d'", $1 );
                                      $$ = strdup( buf );  }
	|	'-' INTNUM					{ char buf[128];
                                      sprintf( buf, "'-%d'", $2 );
                                      $$ = strdup( buf );  }
	|	'?'							{ $$ = "?"; }
	|	NULLX						{ $$ = "";  }
	;

y_assignment:
		NAME COMPARISON y_value				{ sqpStoreAssignment( $1, $3 ); }
	;

y_assignments:
		y_assignment
	|	y_assignments ',' y_assignment
	;

y_comparison:
		NAME COMPARISON y_value				{ $$ = sqpStoreComparison( $1, $2,        $3, NULL ); }
	|	NAME IS NULLX						{ $$ = sqpStoreComparison( $1, "=",       "", NULL ); }
	|	NAME IS NOT NULLX					{ $$ = sqpStoreComparison( $1, "<>",      "", NULL ); }
	|	NAME LIKE y_value y_escape			{ $$ = sqpStoreComparison( $1, "LIKE",    $3, $4   ); }
	|	NAME NOT LIKE y_value y_escape		{ $$ = sqpStoreComparison( $1, "NOTLIKE", $4, $5   ); }
	;

y_escape:
		/* optional */				{ $$ = NULL; }
	|	ESCAPE STRING				{ $$ = $2;   }
	;

y_comparisons:
		y_comparisons OR y_comparisons		{ $$ = sqpStoreCond( sqpor,   $1,   $3,   NULL ); }
	|	y_comparisons AND y_comparisons		{ $$ = sqpStoreCond( sqpand,  $1,   $3,   NULL ); }
	|	NOT y_comparisons					{ $$ = sqpStoreCond( sqpnot,  $2,   NULL, NULL ); }
	|	'(' y_comparisons ')'				{ $$ = sqpStoreCond( sqppar,  $2,   NULL, NULL ); }
	|	y_comparison						{ $$ = sqpStoreCond( sqpcomp, NULL, NULL, $1   ); }
	;

%%


