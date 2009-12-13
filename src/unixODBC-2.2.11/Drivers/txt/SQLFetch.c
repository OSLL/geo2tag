/**********************************************************************
 * SQLFetch
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

SQLRETURN SQLFetch( SQLHSTMT  hDrvStmt)
{
    HDRVSTMT 		hStmt		= (HDRVSTMT)hDrvStmt;

	/* SANITY CHECKS */
	if ( !hStmt )
		return SQL_INVALID_HANDLE;

	sprintf((char*) hStmt->szSqlMsg, "START: hStmt = %p", hStmt );
	logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );

    if ( hStmt->pRowStatusArray )
        (hStmt->pRowStatusArray)[0] = SQL_ROW_NOROW; 

/********************************************************/
/* DRIVER SPECIFIC               						*/
    {
        HRESULTSET      hResultSet;
        HBOUNDCOLUMN    hBoundColumn;

        hResultSet = hStmt->hStmtExtras->hResultSet; 
        if ( !hStmt->hStmtExtras->hResultSet )
        {
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: No result set." );
            return SQL_ERROR;
        }

        /* SCROLL */
        if ( hResultSet->nRow > hResultSet->nRows )
            hResultSet->nRow = 0;

        hResultSet->nRow++;

        if ( hResultSet->nRow > hResultSet->nRows )
        {
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: No data." );
            return SQL_NO_DATA;
        }

        /* HANDLE BOUND COLUMNS */
        for ( hResultSet->nBoundColumn = 1; hResultSet->nBoundColumn <= hResultSet->nBoundColumns; hResultSet->nBoundColumn++ )
        {
            hBoundColumn = hResultSet->hBoundColumns[hResultSet->nBoundColumn - 1];
            if ( SQLGetData_(   hDrvStmt,
                                hBoundColumn->nCol,
                                hBoundColumn->nTargetType,
                                hBoundColumn->pTargetValue,
                                hBoundColumn->nTargetValueMax,
                                hBoundColumn->pnLengthOrIndicator ) != SQL_SUCCESS )
            {
                sprintf((char*) hStmt->szSqlMsg, "END: Failed to get data for column %d", hResultSet->nBoundColumn );
                logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hStmt->szSqlMsg );
                return SQL_ERROR;
            }
        }

        if ( hStmt->pRowStatusArray )
            (hStmt->pRowStatusArray)[0] = SQL_ROW_PROCEED; 
    }
/********************************************************/

	logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success." );
	return SQL_SUCCESS;
}


