#include "sqp.h"

HSQPCOND sqpStoreCond( sqpCondType nType, HSQPCOND pLCond, HSQPCOND pRCond, HSQPCOMPARISON pComp )
{
	HSQPCOND	pCond;
	
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] nType=<%d>\n", __FILE__, __LINE__, nType );
#endif

	pCond               = (HSQPCOND)malloc( sizeof(SQPCOND) );	
	
	pCond->nType		= nType;
	pCond->hLCond		= pLCond;
	pCond->hRCond		= pRCond;
	pCond->hComp		= pComp;
	
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] %s\n", __FILE__, __LINE__, "END." );
#endif

	return( pCond );
}

