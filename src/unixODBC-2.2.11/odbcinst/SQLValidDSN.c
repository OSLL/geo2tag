/**************************************************
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under LGPL 28.JAN.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/
#include <odbcinstext.h>

#define SQL_MAX_DSN_LENGTH 32

BOOL SQLValidDSN(				LPCSTR	pszDSN )
{
	if ( pszDSN == NULL )
		return FALSE;

	if ( strlen( pszDSN ) < 1 || strlen( pszDSN ) > SQL_MAX_DSN_LENGTH )
		return FALSE;

	if ( strstr( pszDSN, "[" ) != NULL )
		return FALSE;

	if ( strstr( pszDSN, "]" ) != NULL )
		return FALSE;

	if ( strstr( pszDSN, "{" ) != NULL )
		return FALSE;

	if ( strstr( pszDSN, "}" ) != NULL )
		return FALSE;

	if ( strstr( pszDSN, "(" ) != NULL )
		return FALSE;

	if ( strstr( pszDSN, ")" ) != NULL )
		return FALSE;

	if ( strstr( pszDSN, "," ) != NULL )
		return FALSE;

	if ( strstr( pszDSN, ";" ) != NULL )
		return FALSE;

	if ( strstr( pszDSN, "?" ) != NULL )
		return FALSE;

	if ( strstr( pszDSN, "*" ) != NULL )
		return FALSE;

	if ( strstr( pszDSN, "=" ) != NULL )
		return FALSE;

	if ( strstr( pszDSN, "!" ) != NULL )
		return FALSE;

	if ( strstr( pszDSN, "@" ) != NULL )
		return FALSE;

	if ( strstr( pszDSN, "\\" ) != NULL )
		return FALSE;


	return TRUE;
}




