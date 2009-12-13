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

BOOL SQLWriteFileDSN(			LPCSTR	pszFileName,
								LPCSTR	pszAppName,
								LPCSTR	pszKeyName,
								LPCSTR	pszString )
{
	HINI	hIni;
	char	szFileName[ODBC_FILENAME_MAX+1];
	char	szFilePath[ODBC_FILENAME_MAX+1];

	if ( pszFileName[0] == '/' )
	{
		strcpy( szFileName, pszFileName );
	}
	else
	{	
        sprintf( szFileName, "%s/ODBCDataSources", odbcinst_system_file_path());
        SQLGetPrivateProfileString( "ODBC", "FILEDSNPATH", szFileName, szFilePath, sizeof( szFilePath ), "odbcinst.ini" );
        sprintf( szFileName, "%s/%s", szFilePath, pszFileName );
	}

    if ( strlen( szFileName ) < 4 || strcmp( szFileName + strlen( szFileName ) - 4, ".dsn" ))
    {
        strcat( szFileName, ".dsn" );
    }

#ifdef __OS2__
	if ( iniOpen( &hIni, szFileName, "#;", '[', ']', '=', TRUE, 0L ) != INI_SUCCESS )
#else
	if ( iniOpen( &hIni, szFileName, "#;", '[', ']', '=', TRUE ) != INI_SUCCESS )
#endif
	{
       	inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_INVALID_PATH, "" );
		return FALSE;
	}

	/* delete section */
	if ( pszString == NULL && pszKeyName == NULL )
	{
		if ( iniObjectSeek( hIni, (char *)pszAppName ) == INI_SUCCESS )
        {
			iniObjectDelete( hIni );
        }
	}
	/* delete entry */
	else if	( pszString == NULL )
	{
		if ( iniPropertySeek( hIni, (char *)pszAppName, (char *)pszKeyName, "" ) == INI_SUCCESS )
        {
			iniPropertyDelete( hIni );
        }
	}
	else
	{
		/* add section */
		if ( iniObjectSeek( hIni, (char *)pszAppName ) != INI_SUCCESS )
        {
			iniObjectInsert( hIni, (char *)pszAppName );
        }
		/* update entry */
		if ( iniPropertySeek( hIni, (char *)pszAppName, (char *)pszKeyName, "" ) == INI_SUCCESS )
		{
			iniObjectSeek( hIni, (char *)pszAppName );
			iniPropertyUpdate( hIni, (char *)pszKeyName, (char *)pszString );
		}
		/* add entry */
		else
		{
			iniObjectSeek( hIni, (char *)pszAppName );
			iniPropertyInsert( hIni, (char *)pszKeyName, (char *)pszString );
		}
	}

	if ( iniCommit( hIni ) != INI_SUCCESS )
	{
		iniClose( hIni );
        inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_REQUEST_FAILED, "" );
		return FALSE;
	}

	iniClose( hIni );

	return TRUE;
}



