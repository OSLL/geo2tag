#include "sqp.h"

void sqpStoreInsert()
{
	HSQPINSERT hInsert = (HSQPINSERT)malloc( sizeof(SQPINSERT) );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	hInsert->pszTable       = g_pszTable;
	hInsert->hValues        = g_hValues;
	hInsert->hColumns       = g_hColumns;
	
	g_pszTable              = NULL;
	g_hValues               = NULL;
	g_hColumns              = NULL;

    g_hParsedSQL            = (HSQPPARSEDSQL)malloc( sizeof(SQPPARSEDSQL) );
    g_hParsedSQL->nType     = sqpinsert;
    g_hParsedSQL->h.hInsert = hInsert;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}


