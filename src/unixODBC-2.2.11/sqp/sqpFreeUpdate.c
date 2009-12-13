#include "sqp.h"

void sqpFreeUpdate( void *pData )
{
	HSQPUPDATE pUpdate = (HSQPUPDATE)pData;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	if ( !pUpdate )
		return;

    if ( pUpdate->hAssignments )
	{
		lstSetFreeFunc( pUpdate->hAssignments, sqpFreeAssignment );
		lstClose( pUpdate->hAssignments );
	}
	if ( pUpdate->hWhere )
	{
		sqpFreeCond( pUpdate->hWhere );
	}
	if ( pUpdate->pszCursor )
		free( pUpdate->pszCursor );
	if ( pUpdate->pszTable )
		free( pUpdate->pszTable );

    free( pUpdate );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}

