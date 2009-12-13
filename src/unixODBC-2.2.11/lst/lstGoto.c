#include "lst.h"

void *lstGoto( HLST hLst, long nIndex )
{
	long n = 0;

    if ( !hLst )
        return NULL;

	lstFirst( hLst );
	while ( (n < nIndex) && (!lstEOL( hLst )) )
	{
		n++;
		lstNext( hLst );
	}

    return hLst->hCurrent;
}



