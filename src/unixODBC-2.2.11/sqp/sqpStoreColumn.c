#include "sqp.h"

void sqpStoreColumn( HLST *ph, char *pszColumn, int nColumn )
{
	HSQPCOLUMN	pColumn	= (HSQPCOLUMN)malloc( sizeof(SQPCOLUMN) );
    char        szName[200];

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN: %s %d\n", __FILE__, __LINE__, pszColumn, nColumn );
#endif

	pColumn->pszTable	= NULL;
    if ( pszColumn )
    	pColumn->pszColumn	= (char*)strdup( pszColumn );
    else
    {
        sprintf( szName, "%d", nColumn );
    	pColumn->pszColumn	= (char*)strdup( szName );
    }

	if ( !(*ph) )
		*ph = lstOpen();

	lstAppend( *ph, pColumn );
	
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif

}

