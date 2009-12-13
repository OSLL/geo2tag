#include "sqp.h"

void sqpFreeParam( void *pData )
{
	HSQPPARAM pParam = (HSQPPARAM)pData;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	if ( !pParam )
		return;
	
	if ( pParam->pszValue )
		free( pParam->pszValue );

    free( pParam );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}

