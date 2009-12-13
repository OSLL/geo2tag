/**********************************************************************
 * SQLNumResultCols
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

SQLRETURN SQLNumResultCols(		SQLHSTMT    hDrvStmt,
								SQLSMALLINT *pnColumnCount )
{
    HDRVSTMT hStmt	= (HDRVSTMT)hDrvStmt;

	/* SANITY CHECKS */
	if ( !hStmt )
		return SQL_INVALID_HANDLE;
	
	sprintf((char*) hStmt->szSqlMsg, "START: hStmt = %p", hStmt );
	logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hStmt->szSqlMsg );

/********************************************************/
/* DRIVER SPECIFIC               						*/
	if ( !hStmt->hStmtExtras->hResultSet )
        *pnColumnCount = 0;
	else
		*pnColumnCount = hStmt->hStmtExtras->hResultSet->nCols;
/********************************************************/

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success." );
    return SQL_SUCCESS;
}



