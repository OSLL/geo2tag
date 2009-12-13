#include "sqp.h"

void sqpFreeAssignment( void *pData )
{
	HSQPASSIGNMENT pAssignment = (HSQPASSIGNMENT)pData;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	if ( !pAssignment )
		return;
	
	if ( pAssignment->pszColumn )
		free( pAssignment->pszColumn );
	if ( pAssignment->pszValue )
		free( pAssignment->pszValue );

    free( pAssignment );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}



