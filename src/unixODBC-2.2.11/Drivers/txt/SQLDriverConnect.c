/**********************************************************************
 * SQLDriverConnect
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

SQLRETURN SQLDriverConnect_(	SQLHDBC   hDrvDbc,
						        SQLCHAR * pszDatabase
                           )
{
	HDRVDBC 	hDbc	= (HDRVDBC)hDrvDbc;

    /* SANITY CHECKS */
    if( SQL_NULL_HDBC == hDbc )
		return SQL_INVALID_HANDLE;

	sprintf((char*) hDbc->szSqlMsg, "START: hDbc=%p pszDatabase=(%s)", hDbc, pszDatabase );
	logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, (char*)hDbc->szSqlMsg );

    if( hDbc->bConnected == 1 )
    {
		logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Already connected" );
        return SQL_ERROR;
    }

/********************************************************/
/* DRIVER SPECIFIC               						*/

    {
        char szDIRECTORY        [INI_MAX_PROPERTY_VALUE+1];
        char szCOLUMNSEPERATOR  [INI_MAX_PROPERTY_VALUE+1];
        char szCATALOG          [INI_MAX_PROPERTY_VALUE+1];

        szDIRECTORY[0]      = '\0';
        szCOLUMNSEPERATOR[0]= '\0';
        szCATALOG[0]        = '\0';
    
        if ( pszDatabase )
            strcpy( szDIRECTORY, (char*)pszDatabase );

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
    
        hDbc->hDbcExtras->pszDatabase = strdup( szDIRECTORY );
    
        if ( *szCOLUMNSEPERATOR )
            hDbc->hDbcExtras->cColumnSeperator = *szCOLUMNSEPERATOR;
        if ( *szCATALOG )
            if ( *szCATALOG == '1' || toupper(*szCATALOG) == 'Y' || toupper(*szCATALOG) == 'O' )
                hDbc->hDbcExtras->bCatalog = 1;
    }
/********************************************************/

	hDbc->bConnected = 1;

    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success" );

    return SQL_SUCCESS;
}

SQLRETURN SQLDriverConnect(		SQLHDBC            hDrvDbc,
								SQLHWND            hWnd,
								SQLCHAR            *szConnStrIn,
								SQLSMALLINT        nConnStrIn,
								SQLCHAR            *szConnStrOut,
								SQLSMALLINT        cbConnStrOutMax,
								SQLSMALLINT        *pnConnStrOut,
								SQLUSMALLINT       nDriverCompletion
                                )
{
	HDRVDBC	hDbc	    = (HDRVDBC)hDrvDbc;
    SQLRETURN nReturn   = SQL_SUCCESS;
    char aNames [20][101];
    char aValues[20][101];
    int  nNames         = 0;
    int  nName          = 0;
    char nChar1         = 0;
    char nChar2         = 0;
    char *psz           = 0;

	/* SANITY CHECKS */
    if( NULL == hDbc )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hDbc->szSqlMsg, "START: hDbc = %p with %s", hDbc, szConnStrIn );
    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, (char*)hDbc->szSqlMsg );

    if( hDbc->bConnected == 1 )
    {
        logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR Already connected" );
        return SQL_ERROR;
    }

    /* 
     * PARSE szConnStrIn
     *
     * ""
     * "DRIVER=Text File;"
     * "DRIVER=Text File; DATABASE=/home/pharvey/database"
     *
     */
    if ( szConnStrIn )
    {
        psz = aNames[0];

        while ( 1 )
        {
            if ( szConnStrIn[nChar1] == '\0' )
            {
                psz[nChar2]             = '\0';
                if ( psz == aNames[nName] )
                    aValues[nName][0]   = '\0';
                break;
            }
            else if ( szConnStrIn[nChar1] == ';' )
            {
                psz[nChar2]             = '\0';
                nChar2                  = 0;
                if ( psz == aNames[nName] )
                    aValues[nName][0]   = '\0';
                nName++;
                psz                     = aNames[nName];
            }
            else if ( szConnStrIn[nChar1] == '=' && psz == aNames[nName] )
            {
                psz[nChar2] = '\0';
                nChar2      = 0;
                psz         = aValues[nName];
            }
            else if ( nChar2 < 100 && !(nChar2 == 0 && isspace( szConnStrIn[nChar1] ) ) )
            {
                if ( psz == aNames[nName] && nChar2 == 0 )
                    nNames++;
                psz[nChar2] = szConnStrIn[nChar1];
                nChar2++;
            }
            nChar1++;
        }
    }

/********************************************************/
/* DRIVER SPECIFIC               						*/
    switch( nDriverCompletion )
    {
    case SQL_DRIVER_NOPROMPT:
        {
            int nDSN        = -1;
            int nDATABASE   = -1;

            /* find DSN */
            for ( nName=0; nName < nNames; nName++ )
            {
                if ( !strcasecmp( "DSN", aNames[nName] ) )
                {
                    nDSN = nName;
                    break;
                }
            }

            /* find DATABASE */
            for ( nName=0; nName < nNames; nName++ )
            {
                if ( !strcasecmp( "DATABASE", aNames[nName] ) )
                {
                    nDATABASE = nName;
                    break;
                }
            }

            if ( nDSN < 0 )
            {
                logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Missing a DSN will connect with defaults." );
                if ( nDATABASE < 0 )
                {
                    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Missing a DATABASE will connect with default." );
                    nReturn = SQLDriverConnect_( hDrvDbc, (SQLCHAR*)"" );
                }
                else
                    nReturn = SQLDriverConnect_( hDrvDbc, (SQLCHAR*)aValues[nDATABASE] );
            }
            else
                nReturn == SQLConnect_( hDrvDbc, (SQLCHAR*)aValues[nDSN], strlen( aValues[nDSN] ), NULL, 0, NULL, 0 );

            if ( nReturn == SQL_SUCCESS || nReturn == SQL_SUCCESS_WITH_INFO )
            {
/* PAH - craps out here... no time or need to fix right now

                strncpy( szConnStrOut, szConnStrIn, cbConnStrOutMax );
                if ( pnConnStrOut )
                    *pnConnStrOut = strlen( szConnStrOut );
*/                    
            }
            else
            {
                logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Failed to connect." );
                return nReturn;
            }
        }
        break;
    case SQL_DRIVER_PROMPT:
    case SQL_DRIVER_COMPLETE:
    case SQL_DRIVER_COMPLETE_REQUIRED:
    default:
        sprintf((char*) hDbc->szSqlMsg, "END: Unsupported nDriverCompletion=%d", nDriverCompletion );
        logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, (char*)hDbc->szSqlMsg );
        return SQL_ERROR;
    }
/********************************************************/

    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success" );

    return nReturn;
}


