#include "sqp.h"

void sqpStoreDelete()
{
    HSQPDELETE hDelete = (HSQPDELETE)malloc( sizeof(SQPDELETE) );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	hDelete->pszTable       = g_pszTable;
	hDelete->hWhere         = g_hConds;
	hDelete->pszCursor      = g_pszCursor;
		
	g_pszTable              = NULL;
	g_hConds                = NULL;
	g_pszCursor             = NULL;

    g_hParsedSQL            = (HSQPPARSEDSQL)malloc( sizeof(SQPPARSEDSQL) );
    g_hParsedSQL->nType     = sqpdelete;
    g_hParsedSQL->h.hDelete = hDelete;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}

