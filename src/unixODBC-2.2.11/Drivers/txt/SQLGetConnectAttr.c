/**********************************************************************
 * SQLGetConnectAttr
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

SQLRETURN  SQLGetConnectAttr(
                             SQLHDBC             hDrvDbc,
                             SQLINTEGER          nAttribute,
                             SQLPOINTER          pValue,
                             SQLINTEGER          nBufferLength,
                             SQLINTEGER          *pnStringLength
                            )
{
	HDRVDBC	hDbc	= (HDRVDBC)hDrvDbc;

	/* SANITY CHECKS */
    if( NULL == hDbc )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hDbc->szSqlMsg, "START: hDbc = %p nAttribute = %d", hDbc, nAttribute );
    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hDbc->szSqlMsg );

/********************************************************/
/* DRIVER SPECIFIC               						*/
    switch ( nAttribute )
    {
    case SQL_ATTR_AUTOCOMMIT:
        *(int*)pValue = SQL_AUTOCOMMIT_ON;
        break;
    case SQL_ATTR_CURRENT_CATALOG:
        strncpy( pValue, hDbc->hDbcExtras->pszDatabase, nBufferLength );
        *pnStringLength = strlen( pValue );
        break;
    case SQL_ATTR_ACCESS_MODE:
    case SQL_ATTR_ASYNC_ENABLE:
    case SQL_ATTR_CONNECTION_TIMEOUT:
    case SQL_ATTR_LOGIN_TIMEOUT:
    case SQL_ATTR_METADATA_ID:
    case SQL_ATTR_ODBC_CURSORS:
    case SQL_ATTR_PACKET_SIZE:
    case SQL_ATTR_QUIET_MODE:
    case SQL_ATTR_TRACE:
    case SQL_ATTR_TRACEFILE:
    case SQL_ATTR_TRANSLATE_LIB:
    case SQL_ATTR_TRANSLATE_OPTION:
    case SQL_ATTR_TXN_ISOLATION:
    default:
        logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Attribute not supported." );
        return SQL_ERROR; 
    }
/********************************************************/

    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success" );

    return SQL_SUCCESS;
}


