#include "sqp.h"

void sqpStoreTable( char *pszTable )
{
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN: %s\n", __FILE__, __LINE__, pszTable );
#endif

	if ( g_pszTable )
    {
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif
		return;
    }
		
	g_pszTable	= (char*)strdup( pszTable );
	
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END:\n", __FILE__, __LINE__ );
#endif

}


