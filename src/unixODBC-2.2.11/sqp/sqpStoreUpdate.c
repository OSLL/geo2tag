#include "sqp.h"

void sqpStoreUpdate()
{
    HSQPUPDATE      hUpdate     = (HSQPUPDATE)malloc( sizeof(SQPUPDATE) );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif


	hUpdate->pszTable		= g_pszTable;
	hUpdate->hAssignments	= g_hAssignments;
	hUpdate->hWhere		    = g_hConds;
	hUpdate->pszCursor	    = g_pszCursor;
	
	g_pszTable              = NULL;
	g_hAssignments          = NULL;
	g_hConds                = NULL;
	g_pszCursor             = NULL;

    g_hParsedSQL            = (HSQPPARSEDSQL)malloc( sizeof(SQPPARSEDSQL) );
    g_hParsedSQL->nType     = sqpupdate;
    g_hParsedSQL->h.hUpdate = hUpdate;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}

