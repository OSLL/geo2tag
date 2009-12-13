#include "sqp.h"

char * sqpError()
{

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN:\n", __FILE__, __LINE__ );
#endif


#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END: %s\n", __FILE__, __LINE__, g_szError );
#endif

	return g_szError;
}


