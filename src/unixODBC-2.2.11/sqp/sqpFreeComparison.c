#include "sqp.h"

void sqpFreeComparison( void *pData )
{
	HSQPCOMPARISON pComparison = (HSQPCOMPARISON)pData;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	if ( !pComparison )
		return;
	
	if ( pComparison->pszLValue )
		free( pComparison->pszLValue );
	if ( pComparison->pszOperator )
		free( pComparison->pszOperator );
	if ( pComparison->pszRValue )
		free( pComparison->pszRValue );

    free( pComparison );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}


