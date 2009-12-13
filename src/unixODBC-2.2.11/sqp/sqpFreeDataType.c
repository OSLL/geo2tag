#include "sqp.h"

void sqpFreeDataType( void *pData )
{
	HSQPDATATYPE hDataType = (HSQPDATATYPE)pData;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN:\n", __FILE__, __LINE__ );
#endif

	if ( !hDataType )
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif
		return;
    }
	
	if ( hDataType->pszType )
		free( hDataType->pszType );

    free( hDataType );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif

}



