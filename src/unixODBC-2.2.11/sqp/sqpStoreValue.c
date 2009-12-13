#include "sqp.h"

void sqpStoreValue( char *pszValue )
{
	char *psz;
    HSQPPARAM       hSQPParam;
	
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	if ( !g_hValues )
		g_hValues = lstOpen();

	if ( !strcmp(pszValue, "?") )
	{
		if ( !lstEOL( g_hParams ) )
		{
			hSQPParam = (HSQPPARAM)lstGet( g_hParams );
			psz = (char *)strdup( hSQPParam->pszValue );
	        lstNext( g_hParams );
		}
		else
			psz = NULL;
	}
	else
	{
		psz			= (char*)strdup(&(pszValue[1]));
		psz[strlen(psz)-1]	= '\0';
	}
	
	lstAppend( g_hValues, psz );

#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}

