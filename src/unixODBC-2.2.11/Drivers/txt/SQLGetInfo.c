/**********************************************************************
 * SQLGetInfo
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

SQLRETURN SQLGetInfo(   SQLHDBC         hDrvDbc,
                        SQLUSMALLINT    nInfoType,
                        SQLPOINTER      pInfoValue,
                        SQLSMALLINT     nInfoValueMax,
                        SQLSMALLINT     *pnLength)
{
	HDRVDBC 	hDbc	= (HDRVDBC)hDrvDbc;

    /* SANITY CHECKS */
    if( !hDbc )
		return SQL_INVALID_HANDLE;

	sprintf((char*) hDbc->szSqlMsg, "START: hDbc = %p nInfoType = %d", hDbc, nInfoType );
	logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hDbc->szSqlMsg );

/********************************************************/
/* DRIVER SPECIFIC               						*/
    switch ( nInfoType )
    {
    case SQL_DRIVER_ODBC_VER:
        strncpy( pInfoValue, "03.00", nInfoValueMax );
        if ( pnLength )
            *pnLength = strlen( pInfoValue );
        break;
    case SQL_CURSOR_COMMIT_BEHAVIOR:
        *(SQLUSMALLINT*)pInfoValue = SQL_CB_PRESERVE;
        break;
    case SQL_CURSOR_ROLLBACK_BEHAVIOR:
        *(SQLUSMALLINT*)pInfoValue = SQL_CB_PRESERVE;
        break;
    case SQL_IDENTIFIER_QUOTE_CHAR:
        strncpy( pInfoValue, "", nInfoValueMax );
        if ( pnLength )
            *pnLength = strlen( pInfoValue );
        break;
    case SQL_CATALOG_NAME_SEPARATOR:
        strncpy( pInfoValue, ".", nInfoValueMax );
        if ( pnLength )
            *pnLength = strlen( pInfoValue );
        break;
    case SQL_SPECIAL_CHARACTERS:
        strncpy( pInfoValue, "", nInfoValueMax );
        if ( pnLength )
            *pnLength = strlen( pInfoValue );
        break;
    case SQL_USER_NAME:
        strncpy( pInfoValue, "", nInfoValueMax );
        if ( pnLength )
            *pnLength = strlen( pInfoValue );
        break;
    case SQL_FILE_USAGE:
        *(SQLUSMALLINT*)pInfoValue = SQL_FILE_CATALOG;
        break;
    case SQL_MAX_CONCURRENT_ACTIVITIES:
        *(SQLUSMALLINT*)pInfoValue = 0;
        break;
    case SQL_DATA_SOURCE_READ_ONLY:
        strncpy( pInfoValue, "N", nInfoValueMax );
        if ( pnLength )
            *pnLength = strlen( pInfoValue );
        break;
    case SQL_DBMS_NAME:
        strncpy( pInfoValue, "unixODBC-TXT", nInfoValueMax );
        if ( pnLength )
            *pnLength = strlen( pInfoValue );
        break;
    case SQL_DBMS_VER:
        strncpy( pInfoValue, "01.00.0000", nInfoValueMax );
        if ( pnLength )
            *pnLength = strlen( pInfoValue );
        break;
    case SQL_NEED_LONG_DATA_LEN:
        strncpy( pInfoValue, "Y", nInfoValueMax );
        if ( pnLength )
            *pnLength = strlen( pInfoValue );
        break;
    case SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1:
        *(SQLUINTEGER*)pInfoValue = 0; /* SQL_CA1_NEXT; */
        break;
    case SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2:
        *(SQLUINTEGER*)pInfoValue = 0;
        break;
    case SQL_KEYSET_CURSOR_ATTRIBUTES1:
        *(SQLUINTEGER*)pInfoValue = 0;  /* SQL_CA1_NEXT */
        break;
    case SQL_KEYSET_CURSOR_ATTRIBUTES2:
        *(SQLUINTEGER*)pInfoValue = 0;
        break;
    case SQL_STATIC_CURSOR_ATTRIBUTES1:
        *(SQLUINTEGER*)pInfoValue = 0;  /* SQL_CA1_NEXT */
        break;
    case SQL_STATIC_CURSOR_ATTRIBUTES2:
        *(SQLUINTEGER*)pInfoValue = 0;
        break;
    case SQL_ALTER_TABLE:
        *(SQLUINTEGER*)pInfoValue = 0;
        break;
    case SQL_OJ_CAPABILITIES:
        *(SQLUINTEGER*)pInfoValue = 0;
        break;
    case SQL_MAX_CHAR_LITERAL_LEN:
        *(SQLUINTEGER*)pInfoValue = 0;
        break;
    case SQL_QUOTED_IDENTIFIER_CASE:
        *(SQLUSMALLINT*)pInfoValue = SQL_IC_SENSITIVE;
        break;
    case SQL_ODBC_SQL_CONFORMANCE:
        *(SQLUSMALLINT*)pInfoValue = SQL_IC_SENSITIVE;
        break;
    case SQL_NON_NULLABLE_COLUMNS:
        *(SQLUSMALLINT*)pInfoValue = SQL_IC_SENSITIVE;
        break;
    case SQL_ORDER_BY_COLUMNS_IN_SELECT:
        strncpy( pInfoValue, "Y", nInfoValueMax );
        if ( pnLength )
            *pnLength = strlen( pInfoValue );
        break;
    case SQL_GROUP_BY:
        *(SQLUSMALLINT*)pInfoValue = SQL_GB_NOT_SUPPORTED;
        break;
    case SQL_MAX_COLUMNS_IN_SELECT:
        *(SQLUSMALLINT*)pInfoValue = 0;
        break;
    case SQL_MAX_TABLES_IN_SELECT:
        *(SQLUSMALLINT*)pInfoValue = 1;
        break;
    case SQL_INTEGRITY:
        strncpy( pInfoValue, "N", nInfoValueMax );
        if ( pnLength )
            *pnLength = strlen( pInfoValue );
        break;
    case SQL_MAX_STATEMENT_LEN:
        *(SQLUINTEGER*)pInfoValue = 0;
        break;
    default:
        logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Unsupported InfoType." );
        return SQL_ERROR;
    }
/********************************************************/

    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success" );
    return SQL_SUCCESS;
}


