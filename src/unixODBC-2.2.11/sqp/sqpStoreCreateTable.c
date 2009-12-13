#include "sqp.h"

void sqpStoreCreateTable()
{
    HSQPCREATETABLE hCreateTable = (HSQPCREATETABLE)malloc( sizeof(SQPCREATETABLE) );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN:\n", __FILE__, __LINE__ );
#endif

	hCreateTable->pszTable          = g_pszTable;
	hCreateTable->hColumnDefs       = g_hColumnDefs;
	
	g_pszTable	    	            = NULL;
	g_hColumnDefs		            = NULL;

    g_hParsedSQL                    = (HSQPPARSEDSQL)malloc( sizeof(SQPPARSEDSQL) );
    g_hParsedSQL->nType             = sqpcreatetable;
    g_hParsedSQL->h.hCreateTable    = hCreateTable;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif

}


