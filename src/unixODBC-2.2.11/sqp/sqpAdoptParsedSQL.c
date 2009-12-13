#include "sqp.h"

/*
 * GIVE THE PARSED SQL TO THE CALLER AND LET THE CALLER
 * FREE IT WHEN IT WANTS (EVEN AFTER sqpClose) WITH sqpFreeParsedSQL
 *
 */
HSQPPARSEDSQL sqpAdoptParsedSQL()
{
    HSQPPARSEDSQL hParsedSQL = g_hParsedSQL;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN:\n", __FILE__, __LINE__ );
#endif

    if ( !g_hParsedSQL )
        printf( "[SQP][%s][%d] DANGER WILL ROBINSON: g_hParsedSQL=NULL\n", __FILE__, __LINE__ );

    g_hParsedSQL = NULL;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif

    return hParsedSQL;
}


