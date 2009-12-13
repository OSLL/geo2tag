#include "sqp.h"

void sqpFreeSelect( void *pData )
{
	HSQPSELECT pSelect = (HSQPSELECT)pData;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	if ( !pSelect )
		return;

	if ( pSelect->hColumns )
	{
		lstSetFreeFunc( pSelect->hColumns, sqpFreeColumn );
		lstClose( pSelect->hColumns );
	}
	if ( pSelect->hWhere )
	{
		sqpFreeCond( pSelect->hWhere );
	}
	if ( pSelect->pszTable )
		free( pSelect->pszTable );
	if ( pSelect->hOrderBy )
	{
		lstSetFreeFunc( pSelect->hOrderBy, sqpFreeColumn );
		lstClose( pSelect->hOrderBy );
	}
	
    free( pSelect );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}


