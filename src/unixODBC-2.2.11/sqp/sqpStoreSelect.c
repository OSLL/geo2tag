#include "sqp.h"

void sqpStoreSelect()
{
	HSQPSELECT      hSelect     = (HSQPSELECT)malloc( sizeof(SQPSELECT) );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	hSelect->pszTable           = g_pszTable;
	hSelect->hColumns           = g_hColumns;
	hSelect->hWhere             = g_hConds;
    hSelect->hOrderBy           = g_hOrderBy;
    hSelect->nOrderDirection    = g_nOrderDirection;
	g_pszTable                  = NULL;
	g_hColumns                  = NULL;
	g_hConds                    = NULL;
    g_hOrderBy                  = NULL;

    g_hParsedSQL                = (HSQPPARSEDSQL)malloc( sizeof(SQPPARSEDSQL) );
    g_hParsedSQL->nType         = sqpselect;
    g_hParsedSQL->h.hSelect     = hSelect;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}


