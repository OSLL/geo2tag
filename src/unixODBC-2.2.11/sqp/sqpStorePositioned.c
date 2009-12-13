#include "sqp.h"

void sqpStorePositioned( char *pszCursor )
{
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	if ( !g_pszCursor )
		g_pszCursor	= (char*)strdup( pszCursor );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}


