/**********************************************************************
 * SQLDescribeCol
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

SQLRETURN SQLDescribeCol( SQLHSTMT    	hDrvStmt,
						  SQLUSMALLINT	nCol,
						  SQLCHAR     	*szColName,
						  SQLSMALLINT 	nColNameMax,
						  SQLSMALLINT 	*pnColNameLength,
						  SQLSMALLINT 	*pnSQLDataType, /* SQL_VARCHAR etc */
						  SQLULEN   	*pnColSize,
						  SQLSMALLINT 	*pnDecDigits,
						  SQLSMALLINT 	*pnNullable )
{
    HDRVSTMT 	hStmt			= (HDRVSTMT)hDrvStmt;

	/* SANITY CHECKS */
	if ( !hStmt )
		return SQL_INVALID_HANDLE;

	sprintf((char*) hStmt->szSqlMsg, "START: hStmt = %p nCol = %d", hStmt, nCol );
	logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, (char*)hStmt->szSqlMsg );

    if ( nCol < 1 )
    {
        logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Column not in resultset." );
        return SQL_ERROR;
    }

/********************************************************/
/* DRIVER SPECIFIC               						*/
    {
        HRESULTSET hResultSet;
        HCOLUMN    hColumn;

        hResultSet = hStmt->hStmtExtras->hResultSet;
    	if ( !hResultSet )
        {
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: No resultset." );
            return SQL_ERROR;
        }
        if ( nCol > hResultSet->nCols )
        {
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Column not in resultset." );
            return SQL_ERROR;
        }
    
        hColumn = (hResultSet->hColumns)[nCol - 1];

    	if ( szColName )
    		strncpy((char*) szColName, hColumn->pszName, nColNameMax );
    	if ( pnColNameLength )
    		*pnColNameLength = strlen((char*) szColName );
    	if ( pnSQLDataType )
    		*pnSQLDataType = hColumn->nType;
    	if ( pnColSize )
    		*pnColSize = hColumn->nLength;
    	if ( pnDecDigits )
    		*pnDecDigits = hColumn->nPrecision;
    	if ( pnNullable )
    		*pnNullable = SQL_NO_NULLS;
    }
/********************************************************/

	logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success." );
	return SQL_SUCCESS;
}

