#include "sqp.h"

void sqpFreeDelete( void *pData )
{
	HSQPDELETE pDelete = (HSQPDELETE)pData;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	if ( !pDelete )
		return;

	if ( pDelete->hWhere )
	{
		sqpFreeCond( pDelete->hWhere );
	}
	if ( pDelete->pszCursor )
		free( pDelete->pszCursor );
	if ( pDelete->pszTable )
		free( pDelete->pszTable );
	
    free( pDelete );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}


