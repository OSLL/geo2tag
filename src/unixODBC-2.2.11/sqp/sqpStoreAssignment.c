#include "sqp.h"

void sqpStoreAssignment( char *pszColumn, char *pszValue )
{
	HSQPASSIGNMENT	pAssignment = (HSQPASSIGNMENT)malloc( sizeof(SQPASSIGNMENT) );
    HSQPPARAM       hSQPParam;
	
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

	pAssignment->pszColumn	= (char*)strdup( pszColumn );
	if ( !strcmp(pszValue, "?") )
	{
		if ( !lstEOL( g_hParams ) )
		{
			hSQPParam = (HSQPPARAM)lstGet( g_hParams );
			pAssignment->pszValue = (char *)strdup( hSQPParam->pszValue );
	        lstNext( g_hParams );
		}
		else
			pAssignment->pszValue = NULL;
	}
	else
	{
		pAssignment->pszValue	= (char*)strdup(&(pszValue[1]));
		pAssignment->pszValue[strlen(pAssignment->pszValue)-1] = '\0';
	}
	
	if ( !g_hAssignments )
		g_hAssignments = lstOpen();

	lstAppend( g_hAssignments, pAssignment );
	
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d]\n", __FILE__, __LINE__ );
#endif

}


