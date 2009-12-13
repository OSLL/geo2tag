#include "sqp.h"

void sqpFreeCond( void *pData )
{
	HSQPCOND pCond = (HSQPCOND)pData;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	if ( !pCond )
		return;
	
	if ( pCond->hLCond )
		sqpFreeCond( pCond->hLCond );
	if ( pCond->hRCond )
		sqpFreeCond( pCond->hRCond );
	if ( pCond->hComp )
		sqpFreeComparison( pCond->hComp );

    free( pCond );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}


