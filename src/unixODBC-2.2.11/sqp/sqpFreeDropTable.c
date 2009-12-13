#include "sqp.h"

void sqpFreeDropTable( void *pData )
{
	HSQPDROPTABLE hDropTable = (HSQPDROPTABLE)pData;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN:\n", __FILE__, __LINE__ );
#endif

	if ( !hDropTable )
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif
		return;
    }
	
    free( hDropTable );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif

}



