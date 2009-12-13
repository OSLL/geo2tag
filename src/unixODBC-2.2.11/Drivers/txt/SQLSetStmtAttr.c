/**********************************************************************
 * SQLSetStmtAttr
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

SQLRETURN  SQLSetStmtAttr(   SQLHSTMT            hDrvStmt,
                             SQLINTEGER          nAttribute,
                             SQLPOINTER          pValue,
                             SQLINTEGER          nStringLength
                         )
{
	HDRVSTMT hStmt = (HDRVSTMT)hDrvStmt;

	/* SANITY CHECKS */
    if( !hStmt )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hStmt->szSqlMsg, "START: hStmt = %p nAttribute = %d", hStmt, nAttribute );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );


/********************************************************/
/* DRIVER SPECIFIC               						*/
    switch ( nAttribute )
    {
    case SQL_ATTR_ROW_STATUS_PTR:
        hStmt->pRowStatusArray = (SQLUSMALLINT*)pValue;
        break;
    case SQL_ATTR_ROW_ARRAY_SIZE:
        if ((int)pValue > 1 )
        {
/*            *(SQLUINTEGER*)pValue = 1; */
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: No support for SQL_ATTR_ROW_ARRAY_SIZE > 1." );
            return SQL_SUCCESS_WITH_INFO;
        }
        break;
    case SQL_ATTR_CURSOR_TYPE:
        break;
    case SQL_ATTR_ASYNC_ENABLE:
    case SQL_ATTR_CONCURRENCY:
    case SQL_ATTR_ENABLE_AUTO_IPD:
    case SQL_ATTR_FETCH_BOOKMARK_PTR:
    case SQL_ATTR_KEYSET_SIZE:
    case SQL_ATTR_MAX_LENGTH:
    case SQL_ATTR_MAX_ROWS:
    case SQL_ATTR_NOSCAN:
    case SQL_ATTR_PARAM_BIND_OFFSET_PTR:
    case SQL_ATTR_PARAM_BIND_TYPE:
    case SQL_ATTR_PARAM_OPERATION_PTR:
    case SQL_ATTR_PARAM_STATUS_PTR:
    case SQL_ATTR_PARAMS_PROCESSED_PTR:
    case SQL_ATTR_PARAMSET_SIZE:
    case SQL_ATTR_QUERY_TIMEOUT:
    case SQL_ATTR_RETRIEVE_DATA:
    case SQL_ATTR_ROW_BIND_OFFSET_PTR:
    case SQL_ATTR_ROW_BIND_TYPE:
    case SQL_ATTR_ROW_NUMBER:
    case SQL_ATTR_ROW_OPERATION_PTR:
    case SQL_ATTR_ROWS_FETCHED_PTR:
    case SQL_ATTR_SIMULATE_CURSOR:
    case SQL_ATTR_USE_BOOKMARKS:
    default:
        logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Attribute not supported." );
        return SQL_ERROR;
    }
/********************************************************/

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success." );
	return SQL_SUCCESS;
}



