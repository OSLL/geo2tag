#include "sqp.h"

void sqpStoreDropTable()
{
    HSQPDROPTABLE hDropTable = (HSQPDROPTABLE)malloc( sizeof(SQPDROPTABLE) );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	hDropTable                      = g_pszTable;
	
	g_pszTable	    	            = NULL;

    g_hParsedSQL                    = (HSQPPARSEDSQL)malloc( sizeof(SQPPARSEDSQL) );
    g_hParsedSQL->nType             = sqpdroptable;
    g_hParsedSQL->h.hDropTable      = hDropTable;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}


