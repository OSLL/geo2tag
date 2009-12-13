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

static void GetEntries(	HINI	hIni,
						LPCSTR	pszSection,
						LPSTR	pRetBuffer,
                        int		nRetBuffer
						)
{
	char	szPropertyName[INI_MAX_PROPERTY_NAME+1];
	char	szValueName[INI_MAX_PROPERTY_NAME+1];
	
	/* COLLECT ALL ENTRIES FOR THE GIVEN SECTION */
	iniObjectSeek( hIni, (char *)pszSection );
	iniPropertyFirst( hIni );

    *pRetBuffer = '\0';

	while ( iniPropertyEOL( hIni ) != TRUE )
	{
		iniProperty( hIni, szPropertyName );
		iniValue( hIni, szValueName );

        if ( strlen( pRetBuffer ) + strlen( szPropertyName ) < nRetBuffer )
        {
            strcat( pRetBuffer, szPropertyName );
            if ( strlen( pRetBuffer ) + 1 < nRetBuffer )
            {
                strcat( pRetBuffer, "=" );
                if ( strlen( pRetBuffer ) + strlen( szValueName ) < nRetBuffer )
                {
                    strcat( pRetBuffer, szValueName );
                    if( strlen( pRetBuffer ) + 1 < nRetBuffer )
                    {
                        strcat( pRetBuffer, ";" );
                    }
                }
            }
        }

		iniPropertyNext( hIni );
	}
}

static void GetSections(	HINI	hIni,
	                LPSTR	pRetBuffer,
                    int		nRetBuffer
					)
{
	int		nStrToCopy;
	char	szObjectName[INI_MAX_OBJECT_NAME+1];

    *pRetBuffer = '\0';

	/* JUST COLLECT SECTION NAMES */
	iniObjectFirst( hIni );
	while ( iniObjectEOL( hIni ) != TRUE )
	{
		iniObject( hIni, szObjectName );

		if ( strcasecmp( szObjectName, "ODBC Data Sources" ) != 0 )
		{
            if ( strlen( pRetBuffer ) + strlen( szObjectName ) + 1 < nRetBuffer )
            {
                strcat( pRetBuffer, szObjectName );
                strcat( pRetBuffer, ";" );
            }
		}
		iniObjectNext( hIni );
	}
}
														
BOOL SQLReadFileDSN(			LPCSTR	pszFileName,
								LPCSTR	pszAppName,
								LPCSTR	pszKeyName,
								LPSTR	pszString,
								WORD	nString,
								WORD	*pnString )
{
	HINI	hIni;
	int		nBufPos			= 0;
	char	szValue[INI_MAX_PROPERTY_VALUE+1];
	char	szFileName[ODBC_FILENAME_MAX+1];
	char	szFilePath[ODBC_FILENAME_MAX+1];
	UWORD	nConfigMode;
    int     ini_done = 0;


	/* SANITY CHECKS */
	if ( pszString == NULL || nString < 1  )
	{
        inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_INVALID_BUFF_LEN, "" );
		return FALSE;
	}
	if ( pszFileName == NULL && pszAppName == NULL && pszKeyName == NULL )
	{
        inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_GENERAL_ERR, "" );
		return FALSE;
	}
	if ( pszAppName == NULL && pszKeyName != NULL )
	{
        inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_INVALID_REQUEST_TYPE, "" );
		return FALSE;
	}

	*pszString = '\0';

	/*****************************************************
	 * GATHER ALL RELEVANT DSN INFORMATION INTO AN hIni
	 *****************************************************/
	if ( pszFileName != 0 && pszFileName[0] == '/' )
	{
        strcpy( szFileName, pszFileName );
        if ( strlen( szFileName ) < 4 || strcmp( szFileName + strlen( szFileName ) - 4, ".dsn" ))
        {
            strcat( szFileName, ".dsn" );
        }

/* on OS/2 the file DSN is a text file */
#ifdef __OS2__
        if ( iniOpen( &hIni, (char*)szFileName, "#;", '[', ']', '=', TRUE, 0L )
                != INI_SUCCESS )
#else
        if ( iniOpen( &hIni, (char*)szFileName, "#;", '[', ']', '=', TRUE )
                != INI_SUCCESS )
#endif
        {
            inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL,
                ODBC_ERROR_INVALID_PATH, "" );

            return FALSE;
        }
	}
	else if ( pszFileName )
	{
        sprintf( szFileName, "%s/ODBCDataSources", odbcinst_system_file_path());
        SQLGetPrivateProfileString( "ODBC", "FILEDSNPATH", szFileName, szFilePath, sizeof( szFilePath ), "odbcinst.ini" );
        sprintf( szFileName, "%s/%s", szFilePath, pszFileName );

        if ( strlen( szFileName ) < 4 || strcmp( szFileName + strlen( szFileName ) - 4, ".dsn" ))
        {
            strcat( szFileName, ".dsn" );
        }

/* on OS/2 the file DSN is a text file */
#ifdef __OS2__
        if ( iniOpen( &hIni, (char*) szFileName, "#;", '[', ']', '=', TRUE, 0L )
                != INI_SUCCESS )
#else
        if ( iniOpen( &hIni, (char*) szFileName, "#;", '[', ']', '=', TRUE )
                != INI_SUCCESS )
#endif
        {
            inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL,
                    ODBC_ERROR_INVALID_PATH, "" );

            return FALSE;
        }
	}
	
	if ( pszAppName == NULL && pszKeyName == NULL )
	{
		GetSections( hIni, pszString, nString );
	}
    else if ( pszAppName != NULL && pszKeyName == NULL )
	{
		GetEntries( hIni, pszAppName, pszString, nString );
	}
	else
	{
		/* TRY TO GET THE ONE ITEM MATCHING Section & Entry */
		if ( iniPropertySeek( hIni, (char *)pszAppName, (char *)pszKeyName, "" ) != INI_SUCCESS )
		{
            inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL,
                    ODBC_ERROR_REQUEST_FAILED, "" );

            return FALSE;
		}
		else
		{
			iniValue( hIni, szValue );
			strncpy( pszString, szValue, nString );
            pszString[ nString - 1 ] = '\0';
			nBufPos = strlen( szValue );
		}
	}

    if ( pszFileName )
    {
        iniClose( hIni );
    }

	return TRUE;
}
