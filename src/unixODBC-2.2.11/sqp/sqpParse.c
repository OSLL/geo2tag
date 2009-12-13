#include "sqp.h"


int sqpParse()
{
    int n;

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN:\n", __FILE__, __LINE__ );
#endif

    n = yyparse();

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END: %d\n", __FILE__, __LINE__, n );
#endif

    return n;
}


