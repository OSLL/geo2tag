#include "sqp.h"

void sqpFreeCreateTable( void *pData )
{
	HSQPCREATETABLE pCreateTable = (HSQPCREATETABLE)pData;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	if ( !pCreateTable )
		return;
	
	if ( pCreateTable->pszTable )
		free( pCreateTable->pszTable );
	if ( pCreateTable->hColumnDefs )
    {
        lstSetFreeFunc( pCreateTable->hColumnDefs, sqpFreeColumnDef );
        lstClose( pCreateTable->hColumnDefs );
    }

    free( pCreateTable );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}



