/**************************************************
 * _SQLGetInstalledDrivers
 *
 * Added to allow ODBC Config programs and the ODBC
 * driver manager to access system information without
 * having to worry about where it is... just like accessing
 * Data Source information. So no surprise... its just
 * like SQLGetPrivateProfileString()!
 *
 * see SQLGetPrivateProfileString to see how this is called.
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

int _SQLGetInstalledDrivers(    LPCSTR  pszSection,
                                LPCSTR  pszEntry,
                                LPCSTR  pszDefault,
                                LPCSTR  pRetBuffer,
                                int     nRetBuffer )
{
    HINI    hIni;
    int     nBufPos         = 0;
    int     nStrToCopy;
    char    szObjectName[INI_MAX_OBJECT_NAME+1];
    char    szPropertyName[INI_MAX_PROPERTY_NAME+1];
    char    szValue[INI_MAX_PROPERTY_VALUE+1];
    char    szIniName[ INI_MAX_OBJECT_NAME + 1 ];
    char    *ptr;

    /* SANITY CHECKS */
    if ( pRetBuffer == NULL || nRetBuffer < 2 )
    {
        inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_GENERAL_ERR, "" );
        return -1;
    }

#ifdef VMS
    sprintf( szIniName, "%sODBCINST.INI", odbcinst_system_file_path() );
#else
    sprintf( szIniName, "%s/odbcinst.ini", odbcinst_system_file_path() );
#endif

    /* PROCESS ODBC INI FILE */
#ifdef __OS2__
    if ( iniOpen( &hIni, szIniName, "#;", '[', ']', '=', TRUE, 1L ) != INI_SUCCESS )
#else
    if ( iniOpen( &hIni, szIniName, "#;", '[', ']', '=', TRUE ) != INI_SUCCESS )
#endif
    {
        inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_COMPONENT_NOT_FOUND, "" );
        return -1;
    }

    nBufPos = 0;
    if ( pszSection == NULL )
    {
        ptr = (char*) pRetBuffer;
        *ptr = '\0';

        /* JUST COLLECT SECTION NAMES */

        for( iniObjectFirst( hIni ); iniObjectEOL( hIni ) != TRUE; iniObjectNext( hIni ))
        {
            iniObject( hIni, szObjectName );

            if ( strcasecmp( szObjectName, "ODBC" ) == 0 )
            {
                continue;
            }
            else if ( nBufPos + 1 + strlen( szObjectName ) >= nRetBuffer )
            {
                break;
            }
            else
            {
                strcpy( ptr, szObjectName );
                ptr += strlen( ptr ) + 1;
                nBufPos += strlen( szObjectName ) + 1;
            }
        }

        /*
         * Add final NULL
         */

        if ( nBufPos == 0 )
        {
            ptr ++;
        }

        *ptr = '\0';
    }
    else if ( pszEntry == NULL )
    {
        ptr = (char*) pRetBuffer;
        *ptr = '\0';

        iniObjectSeek( hIni, (char *)pszSection );

        /* COLLECT ALL ENTRIES FOR THE GIVEN SECTION */

        for( iniPropertyFirst( hIni ); iniPropertyEOL( hIni ) != TRUE; iniPropertyNext( hIni ))
        {
            iniProperty( hIni, szPropertyName );

            if ( nBufPos + 1 + strlen( szPropertyName ) >= nRetBuffer )
            {
                break;
            }
            else
            {
                strcpy( ptr, szPropertyName );
                ptr += strlen( ptr ) + 1;
                nBufPos += strlen( szPropertyName ) + 1;
            }
        }

        /*
         * Add final NULL
         */

        if ( nBufPos == 0 )
        {
            ptr ++;
        }
    }
    else
    {
        /* TRY TO GET THE ONE ITEM MATCHING Section & Entry */
        if ( iniPropertySeek( hIni, (char *)pszSection, (char *)pszEntry, "" ) != INI_SUCCESS )
        {
            strncpy( (char *)pRetBuffer, pszDefault, nRetBuffer );
            ((char*)pRetBuffer)[ nRetBuffer - 1 ] = '\0';
        }
        else
        {
            iniValue( hIni, szValue );
            nStrToCopy = strlen( szValue ) + 1;                 /* factor NULL terminator for string */
            if ( nBufPos + nStrToCopy + 1 > nRetBuffer )        /* factor NULL terminator for buffer */
                nStrToCopy = nRetBuffer - nBufPos - 2;
            strncpy( (char *)&(pRetBuffer[nBufPos]), szValue, nStrToCopy );
            nBufPos += nStrToCopy;
        }
    }

    /* CLOSE */
    iniClose( hIni );

    return nBufPos;
}


