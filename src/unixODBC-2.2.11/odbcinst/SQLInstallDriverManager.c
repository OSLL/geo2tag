/*************************************************
 * SQLInstallDriverManager
 *
 * I return the default dir for core components.. but
 * thats it.
 * This may differ slightly from the spec.
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

BOOL SQLInstallDriverManager(	LPSTR	pszPath,
								WORD	nPathMax,
								WORD	*pnPathOut )
{
    char  szIniName[ INI_MAX_OBJECT_NAME + 1 ];

	/* SANITY CHECKS */
	if ( pszPath == NULL || nPathMax < 2 )
	{
        inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_GENERAL_ERR, "" );
		return 0;
	}

    sprintf( szIniName, "%s", odbcinst_system_file_path() );

	/* DO SOMETHING */
	strncpy( pszPath, szIniName, nPathMax );
	if ( pnPathOut != NULL )
		*pnPathOut = strlen( pszPath );

	return TRUE;
}


