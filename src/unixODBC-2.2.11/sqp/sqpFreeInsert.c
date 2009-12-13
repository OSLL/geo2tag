#include "sqp.h"

void sqpFreeInsert( void *pData )
{
	HSQPINSERT pInsert = (HSQPINSERT)pData;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	if ( !pInsert )
		return;
	
	if ( pInsert->hValues )
		lstClose( pInsert->hValues );
	if ( pInsert->pszTable )
		free( pInsert->pszTable );
	if ( pInsert->hColumns )
	{
		lstSetFreeFunc( pInsert->hColumns, sqpFreeColumn );
		lstClose( pInsert->hColumns );
	}
	
    free( pInsert );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}

