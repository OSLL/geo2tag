/**********************************************************************
 * SQLGetDiagRec
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

SQLRETURN  SQLGetDiagRec(    SQLSMALLINT    nHandleType,
                             SQLHANDLE      hHandle,
                             SQLSMALLINT    nRecordNumber,
                             SQLCHAR *      pszState,
                             SQLINTEGER *   pnNativeError,
                             SQLCHAR *      pszMessageText,
                             SQLSMALLINT    nBufferLength,
                             SQLSMALLINT *  pnStringLength
                        )
{
	char	szMsgHdr[1024];
	char	szMsg[1024];
	int		nCode;

    /* SANITY CHECKS */
    if ( !hHandle )
        return SQL_INVALID_HANDLE;

	/* DEFAULTS */
    if ( pszState )
        strcpy((char*) pszState, "-----" );

    if ( pnNativeError )
        *pnNativeError = 0;

    if ( pszMessageText )
        memset( pszMessageText, 0, nBufferLength );

    if ( pnStringLength )
        *pnStringLength = 0;
    
    /* PROCESS HANDLE TYPE */
    switch ( nHandleType )
    {
    case SQL_HANDLE_ENV:
        {
            HDRVENV		hEnv	= (HDRVENV)hHandle;

            if ( logPopMsg( hEnv->hLog, szMsgHdr, &nCode, szMsg ) != LOG_SUCCESS )
                return SQL_NO_DATA;
        }
        break;
    case SQL_HANDLE_DBC:
        {
            HDRVDBC		hDbc	= (HDRVDBC)hHandle;

            if ( logPopMsg( hDbc->hLog, szMsgHdr, &nCode, szMsg ) != LOG_SUCCESS )
                return SQL_NO_DATA;
        }
        break;
    case SQL_HANDLE_STMT:
        {
            HDRVSTMT	hStmt	= (HDRVSTMT)hHandle;

            if ( logPopMsg( hStmt->hLog, szMsgHdr, &nCode, szMsg ) != LOG_SUCCESS )
                return SQL_NO_DATA;
        }
        break;
    case SQL_HANDLE_DESC:
    default:

        return SQL_ERROR;
    }


    if ( pnNativeError )
        *pnNativeError = nCode;

    if ( pszMessageText )
        strncpy((char*) pszMessageText, szMsg, nBufferLength-1 );

    if ( pnStringLength )
        *pnStringLength = strlen((char*) pszMessageText );

    return SQL_SUCCESS;
}


