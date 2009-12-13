/**********************************************************************
 * SQLConnect
 *
 **********************************************************************
 *
 * This code was created by Peter Harvey (mostly during Christmas 98/99).
 * This code is LGPL. Please ensure that this message remains in future
 * distributions and uses of this code (thats about all I get out of it).
 * - Peter Harvey pharvey@codebydesign.com
 *
 **********************************************************************/

#include "driver.h"

SQLRETURN SQLConnect_(	SQLHDBC        hDrvDbc,
						SQLCHAR        *szDataSource,
						SQLSMALLINT    nDataSourceLength,
						SQLCHAR        *szUID,
						SQLSMALLINT    nUIDLength,
						SQLCHAR        *szPWD,
						SQLSMALLINT    nPWDLength
                          )
{
	HDRVDBC 	hDbc	= (HDRVDBC)hDrvDbc;

    char    	szTRACE             [INI_MAX_PROPERTY_VALUE+1];
    char    	szTRACEFILE         [INI_MAX_PROPERTY_VALUE+1];

    /* SANITY CHECKS */
    if( SQL_NULL_HDBC == hDbc )
		return SQL_INVALID_HANDLE;

	sprintf((char*) hDbc->szSqlMsg, "START: hDbc=%p szDataSource=(%s)", hDbc, szDataSource );
	logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hDbc->szSqlMsg );

    if( hDbc->bConnected == 1 )
    {
		logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Already connected" );
        return SQL_ERROR;
    }

    if ( strlen((char*) szDataSource ) > ODBC_FILENAME_MAX+INI_MAX_OBJECT_NAME )
    {
		logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Given Data Source is too long. I consider it suspect." );
        return SQL_ERROR;
    }

    /********************
     * gather and use any required DSN properties
	 * - DATABASE
     ********************/
    szTRACE[0] 			= '\0';
    szTRACEFILE[0] 		= '\0';
	SQLGetPrivateProfileString((char*) szDataSource, "TRACE", "", szTRACE, sizeof(szTRACE), ".odbc.ini" );
	SQLGetPrivateProfileString((char*) szDataSource, "TRACEFILE", "", szTRACEFILE, sizeof(szTRACEFILE), ".odbc.ini" );

    /********************
     * SET TRACING ON HERE
	 * - SQLDisconnect WILL TURN IT OFF (by freeing pszLogFile)
	 * - Subordinate handles (ie HSTMT) will inherit the trace
     ********************/
	if ( szTRACE )
	{
		if ( szTRACE[0] == '1' || toupper(szTRACE[0]) == 'Y' || toupper(szTRACE[0]) == 'O' )
		{
			if ( szTRACEFILE && *szTRACEFILE )
			{
                if ( hDbc->hLog->pszLogFile )
                    free( hDbc->hLog->pszLogFile );
				hDbc->hLog->pszLogFile = (char*)strdup( szTRACEFILE );
			}
		}
	}

/********************************************************/
/* DRIVER SPECIFIC               						*/

    /* AVOID COMPILE WARNINGS */
    {
        char * sz;
        sz = (char*)szUID; /* could use these to suid but not worth the security risk */
        sz = (char*)szPWD;
    }

    {
        char szDIRECTORY        [INI_MAX_PROPERTY_VALUE+1];
        char szCOLUMNSEPERATOR  [INI_MAX_PROPERTY_VALUE+1];
        char szCATALOG          [INI_MAX_PROPERTY_VALUE+1];
        char szCASESENSITIVE    [INI_MAX_PROPERTY_VALUE+1];

        szDIRECTORY[0]      = '\0';
        szCOLUMNSEPERATOR[0]= '\0';
        szCATALOG[0]        = '\0';
        szCASESENSITIVE[0]  = '\0';
    	SQLGetPrivateProfileString((char*) szDataSource, "DIRECTORY", "", szDIRECTORY, sizeof(szDIRECTORY), ".odbc.ini" );
    	SQLGetPrivateProfileString((char*) szDataSource, "COLUMNSEPERATOR", "", szCOLUMNSEPERATOR, sizeof(szCOLUMNSEPERATOR), ".odbc.ini" );
    	SQLGetPrivateProfileString((char*) szDataSource, "CATALOG", "", szCATALOG, sizeof(szCATALOG), ".odbc.ini" );
    	SQLGetPrivateProfileString((char*) szDataSource, "CASESENSITIVE", "", szCASESENSITIVE, sizeof(szCASESENSITIVE), ".odbc.ini" );
    
        if ( ! *szDIRECTORY )
        {
            /* default to ~/.odbctxt */
            uid_t           nUserID     = getuid();
            struct passwd * pPasswd     = (struct passwd *)getpwuid( nUserID );
            char *          pHomeDir    = NULL;
    
            if ( pPasswd != NULL )
                    if ( (char *)pPasswd->pw_dir != NULL )
                            pHomeDir = pPasswd->pw_dir;
            if ( pHomeDir )
            {
                sprintf( szDIRECTORY, "%s/.odbctxt", pHomeDir );
                mkdir( szDIRECTORY, S_IRUSR | S_IWUSR | S_IXUSR );
                hDbc->hDbcExtras->pszDirectory = strdup( szDIRECTORY );
            }
            else
            {
                logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Could not determine table file directory." );
                return SQL_ERROR;
            }
    
        }
        else
            hDbc->hDbcExtras->pszDirectory = strdup( szDIRECTORY );
    
        hDbc->hDbcExtras->pszDatabase = strdup((char*) szDataSource );
    
        if ( *szCOLUMNSEPERATOR )
            hDbc->hDbcExtras->cColumnSeperator = *szCOLUMNSEPERATOR;
        if ( *szCATALOG )
            if ( *szCATALOG == '1' || toupper(*szCATALOG) == 'Y' || toupper(*szCATALOG) == 'O' )
                hDbc->hDbcExtras->bCatalog = 1;
        if ( *szCASESENSITIVE )
            if ( *szCASESENSITIVE == '0' || toupper(*szCASESENSITIVE) == 'N' )
                hDbc->hDbcExtras->bCaseSensitive = 0;
    }
/********************************************************/

	hDbc->bConnected = 1;

    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success" );

    return SQL_SUCCESS;
}


SQLRETURN SQLConnect(	SQLHDBC        hDrvDbc,
						SQLCHAR        *szDataSource,
						SQLSMALLINT    nDataSourceLength,
						SQLCHAR        *szUID,
						SQLSMALLINT    nUIDLength,
						SQLCHAR        *szPWD,
						SQLSMALLINT    nPWDLength
                          )
{
    return SQLConnect_( hDrvDbc, szDataSource, nDataSourceLength, szUID, nUIDLength, szPWD, nPWDLength );
}
