#include "sqp.h"

HSQPCOMPARISON sqpStoreComparison( char *pszLValue, char *pszOperator, char *pszRValue, char *pszEscape )
{
	HSQPCOMPARISON	pComparison;
    HSQPPARAM       hSQPParam;
	
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] BEGIN: %s %s %s %s\n", __FILE__, __LINE__, pszLValue, pszOperator, pszRValue, pszEscape );
#endif

	pComparison                 = (HSQPCOMPARISON)malloc( sizeof(SQPCOMPARISON) );	
	
	pComparison->pszLValue		= (char*)strdup( pszLValue );
	pComparison->pszOperator	= (char*)strdup( pszOperator );
    if ( !strcmp(pszRValue, "?") )
    {
        pComparison->pszRValue = NULL;
        if ( !lstEOL( g_hParams ) )
        {
            hSQPParam = (HSQPPARAM)lstGet( g_hParams );
            pComparison->pszRValue = (char *)strdup( hSQPParam->pszValue );
            lstNext( g_hParams );
        }
    }
    else
    {
        pComparison->pszRValue      = (char*)strdup(&(pszRValue[1]));
        pComparison->pszRValue[strlen(pComparison->pszRValue)-1]= '\0';
    }
    if (pszEscape)
        pComparison->cEscape    = pszEscape[1];
    else
        pComparison->cEscape    = '\0';
	
#ifdef SQPDEBUG
	printf( "[SQP][%s][%d] END: %s %s %s\n", __FILE__, __LINE__, pComparison->pszLValue, pComparison->pszOperator, pComparison->pszRValue );
#endif

    return( pComparison );
}

