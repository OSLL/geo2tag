/**********************************************************************
 * SQLGetStmtAttr
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

SQLRETURN  SQLGetStmtAttr(   SQLHSTMT            hDrvStmt,
                             SQLINTEGER          nAttribute,
                             SQLPOINTER          pValue,
                             SQLINTEGER          nBufferLength,
                             SQLINTEGER         *pnStringLength
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
    case SQL_ATTR_APP_ROW_DESC:
    case SQL_ATTR_APP_PARAM_DESC:
    case SQL_ATTR_IMP_ROW_DESC:
    case SQL_ATTR_IMP_PARAM_DESC:
    default:
        logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Attribute not supported." );
        return SQL_ERROR;
    }
    
/********************************************************/

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success." );
	return SQL_SUCCESS;
}


