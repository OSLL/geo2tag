#include "sqp.h"


/*
 * Extern these in sqp.h and instantiate here. 
 *
 * Each process that uses the sqp share will get exactly one version of 
 * these. So sqp is NOT multi-thread safe but is safe across multi 
 * processes.
 *
 */
char		    g_szError[1024];

HSQPPARSEDSQL   g_hParsedSQL;

char *		    g_pszTable;
char *   	    g_pszType;
HLST		    g_hColumns;
HLST		    g_hColumnDefs;
HSQPDATATYPE    g_hDataType;
HLST		    g_hValues;
HLST		    g_hAssignments;
HLST            g_hOrderBy;
sqpOrder        g_nOrderDirection;
char *		    g_pszCursor;
int             g_nNulls;

char *		    g_pszSQLCursor;		/* yyparse position init to start of SQL string before yyparse	*/
char *		    g_pszSQLLimit;		/* ptr to NULL terminator of SQL string (yyparse stops here)	*/
int 		    g_nLineNo;

HLST            g_hParams;
HSQPCOND        g_hConds;

void sqpOpen( char *pszFirstChar, char *pszLastChar, HLST hParams )
{

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN:\n", __FILE__, __LINE__ );
#endif

	*g_szError		    = '\0';

    g_hParsedSQL        = NULL;

	g_pszTable		    = NULL;
	g_pszType		    = NULL;
	g_hColumns		    = NULL;
	g_hColumnDefs	    = NULL;
    g_hDataType         = NULL;
	g_hValues		    = NULL;
	g_hAssignments	    = NULL;
    g_hOrderBy          = NULL;
    g_nOrderDirection   = sqpnone;
	g_pszCursor		    = NULL;
    g_nNulls            = 1;

	g_pszSQLCursor	    = pszFirstChar;
	g_pszSQLLimit	    = pszLastChar;
	g_nLineNo		    = 1;

	g_hParams		    = hParams;
    lstFirst( g_hParams );
	g_hConds		    = NULL;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif

}


