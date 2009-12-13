#include "sqp.h"

/*
 * HOPEFULLY CLEANS UP A FAILED PARSE (if it did fail)
 *
 */
void sqpClose()
{

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN:\n", __FILE__, __LINE__ );
#endif

	*g_szError		= '\0';

    /* 
     * This should only do it if sqpParse worked but the caller did
     * not adopt the result.
     *
     */
    if ( sqpFreeParsedSQL( g_hParsedSQL ) )
    {
        g_hParsedSQL = NULL;
#ifdef SQPDEBUG
    	printf( "[SQP][%s][%d] END: Appears that parse made it to top level struct and it has been freed ok.\n", __FILE__, __LINE__ );
#endif
        return;
    }

    /*
     * Free any work in progress (sql parts which never made it into 
     * a parent struct). This is required only when a sqpParse fails
     * but we can safely do it in anycase by checking for values before
     * calling free.
     *
     */
    if ( g_pszTable )
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] Free g_pszTable\n", __FILE__, __LINE__ );
#endif
        free(g_pszTable);
    }

    if ( g_pszType )
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] Free g_pszType\n", __FILE__, __LINE__ );
#endif
        free(g_pszType);
    }

    if ( g_hColumnDefs )
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] Free g_hColumnDefs\n", __FILE__, __LINE__ );
#endif
        lstSetFreeFunc( g_hColumnDefs, sqpFreeColumnDef );
        lstClose( g_hColumnDefs );
    }
    if ( g_hDataType )
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] Free g_hDataType\n", __FILE__, __LINE__ );
#endif
        sqpFreeDataType( g_hDataType );
    }
    if ( g_hColumns )
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] Free g_hColumns\n", __FILE__, __LINE__ );
#endif
        lstSetFreeFunc( g_hColumns, sqpFreeColumn );
        lstClose( g_hColumns );
    }
    if ( g_hValues )
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] Free g_hValues\n", __FILE__, __LINE__ );
#endif
        lstClose( g_hValues );
    }

    if ( g_hAssignments	)
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] Free g_hAssignments\n", __FILE__, __LINE__ );
#endif
        lstSetFreeFunc( g_hAssignments, sqpFreeAssignment );
        lstClose( g_hAssignments );
    }
    if ( g_hOrderBy )
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] Free g_hOrderBy\n", __FILE__, __LINE__ );
#endif
        lstSetFreeFunc( g_hOrderBy, sqpFreeColumn );
        lstClose( g_hOrderBy );
    }
    if ( g_pszCursor )
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] Free g_pszCursor\n", __FILE__, __LINE__ );
#endif
        free(g_pszCursor);
    }

    g_hParams = NULL;
    if ( g_hConds )
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] Free g_hConds\n", __FILE__, __LINE__ );
#endif
        sqpFreeCond( g_hConds );
        g_hConds = NULL;
    }

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END: Hopefully; memory is cleaned up.\n", __FILE__, __LINE__ );
#endif

}


