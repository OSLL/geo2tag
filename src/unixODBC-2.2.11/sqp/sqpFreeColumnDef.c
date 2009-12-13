#include "sqp.h"

void sqpFreeColumnDef( void *pData )
{
	HSQPCOLUMNDEF pColumnDef = (HSQPCOLUMNDEF)pData;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN:\n", __FILE__, __LINE__ );
#endif

	if ( !pColumnDef )
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif
		return;
    }
	
	if ( pColumnDef->pszColumn )
		free( pColumnDef->pszColumn );

    sqpFreeDataType( pColumnDef->hDataType );

    free( pColumnDef );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif

}



