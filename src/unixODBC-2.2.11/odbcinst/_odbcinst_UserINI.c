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
#ifdef HAVE_PWD_H
#include <pwd.h>
#endif

#include <odbcinstext.h>

#ifdef VMS

BOOL _odbcinst_UserINI( char *pszFileName, BOOL bVerify )
{
	FILE			*hFile;
	char    		*szEnv_INIUSER				= getvmsenv("ODBCINI");
	struct passwd	*pPasswd					= NULL;
	char			*pHomeDir					= NULL;

    pszFileName[0] = '\0';

	if ( szEnv_INIUSER )
	{
		strncpy( pszFileName, szEnv_INIUSER, ODBC_FILENAME_MAX );
	}
    else
	{
		sprintf( pszFileName, "SYS$LOGIN:ODBC.INI" );
	}

	if ( bVerify )
	{
		hFile = uo_fopen( pszFileName, "r" );
		if ( hFile )
			uo_fclose( hFile );
		else
			return FALSE;
	}

	return TRUE;
}

#else

BOOL _odbcinst_UserINI( char *pszFileName, BOOL bVerify )
{
	FILE			*hFile;
	char    		*szEnv_INIUSER				= getenv("ODBCINI");
	uid_t			nUserID						= getuid();
	struct passwd	*pPasswd					= NULL;
	char			*pHomeDir					= NULL;

	pHomeDir	= "/home";		   		 				
	pPasswd		= (struct passwd *)getpwuid(nUserID);	
	
    pszFileName[0] = '\0';

	if ( pPasswd != NULL )
		if ( ( char *)pPasswd->pw_dir != NULL )
			pHomeDir	= pPasswd->pw_dir;

	if ( szEnv_INIUSER )
	{
		strncpy( pszFileName, szEnv_INIUSER, ODBC_FILENAME_MAX );
	}
	if ( pszFileName[0] == '\0' )
	{
		sprintf( pszFileName, "%s%s", pHomeDir, "/.odbc.ini" );
	}

#ifdef DHAVE_ENDPWENT
    /*
     * close the password file
     */
    endpwent();
#endif

	if ( bVerify )
	{
        /*
         * create it of it doesn't exist
         */

		hFile = uo_fopen( pszFileName, "a" );
		if ( hFile )
			uo_fclose( hFile );
		else
			return FALSE;
	}

	return TRUE;
}

#endif

