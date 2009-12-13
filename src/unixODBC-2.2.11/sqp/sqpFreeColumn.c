#include "sqp.h"

void sqpFreeColumn( void *pData )
{
	HSQPCOLUMN pColumn = (HSQPCOLUMN)pData;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN:\n", __FILE__, __LINE__ );
#endif

	if ( !pColumn )
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif
		return;
    }
	
	if ( pColumn->pszColumn )
		free( pColumn->pszColumn );
	if ( pColumn->pszTable )
		free( pColumn->pszTable );

    free( pColumn );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif

}

