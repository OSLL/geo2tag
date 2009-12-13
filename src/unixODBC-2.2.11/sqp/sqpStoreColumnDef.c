#include "sqp.h"

void sqpStoreColumnDef( char *pszColumn )
{
	HSQPCOLUMNDEF	pColumnDef = (HSQPCOLUMNDEF)malloc( sizeof(SQPCOLUMNDEF) );
	
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN: %s\n", __FILE__, __LINE__, pszColumn );
#endif

	pColumnDef->pszColumn	= (char*)strdup( pszColumn );
    pColumnDef->hDataType   = g_hDataType;
    pColumnDef->bNulls      = g_nNulls;

    g_hDataType             = 0;
    g_nNulls                = 1;

	if ( !g_hColumnDefs )
		g_hColumnDefs = lstOpen();

	lstAppend( g_hColumnDefs, pColumnDef );
	
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif

}


