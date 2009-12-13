/**********************************************************************
 * SQLBindCol
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

/* THIS IS THE CORRECT SYNTAX....
SQLRETURN SQLBindCol(	SQLHSTMT        hDrvStmt,
						SQLUSMALLINT    nCol,
						SQLSMALLINT     nTargetType,
						SQLPOINTER      pTargetValue,
						SQLLEN      nTargetValueMax,
						SQLLEN      *pnLengthOrIndicator )
AND THIS IS WHAT WORKS... */						
SQLRETURN   SQLBindCol(SQLHSTMT hDrvStmt,
		   SQLUSMALLINT nCol, SQLSMALLINT nTargetType,
		   SQLPOINTER pTargetValue, SQLLEN nTargetValueMax,
	   	   SQLLEN *pnLengthOrIndicator)
{
    HDRVSTMT 		hStmt	= (HDRVSTMT)hDrvStmt;

    /* SANITY CHECKS */
    if( NULL == hStmt )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hStmt->szSqlMsg, "hStmt=%p nCol=%5d", hStmt, nCol );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hStmt->szSqlMsg );

	if ( nCol < 1 )
	{
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "Desired column is less than 1." );
		return SQL_ERROR;
	}

/********************************************************/
/* DRIVER SPECIFIC               						*/
    {
        HRESULTSET      hResultSet;
        HBOUNDCOLUMN    hBoundColumn;

        hResultSet = hStmt->hStmtExtras->hResultSet;
    	if ( !hResultSet )
    	{
    		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR No result set." );
    		return SQL_ERROR;
    	}
    
    	if ( nCol > hResultSet->nCols )
    	{
    		sprintf((char*) hStmt->szSqlMsg, "SQL_ERROR Column %d is out of range. Range is 1 - %ld", nCol, hResultSet->nCols );
    		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hStmt->szSqlMsg );
    		return SQL_ERROR;
    	}
    
    	if ( pTargetValue == NULL )
    	{
    		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR Invalid data pointer" );
    		return SQL_ERROR;
    	}
    
    	if ( pnLengthOrIndicator )
    		*pnLengthOrIndicator = 0;
    
    	hBoundColumn						= (HBOUNDCOLUMN)calloc( 1, sizeof(BOUNDCOLUMN) );
        hBoundColumn->nCol                  = nCol;
    	hBoundColumn->nTargetType			= nTargetType;
    	hBoundColumn->nTargetValueMax		= nTargetValueMax;
    	hBoundColumn->pnLengthOrIndicator	= pnLengthOrIndicator;
    	hBoundColumn->pTargetValue			= pTargetValue;

        hResultSet->nBoundColumns++;
        hResultSet->hBoundColumns = (HBOUNDCOLUMNS)realloc( hResultSet->hBoundColumns, sizeof(HBOUNDCOLUMN) * hResultSet->nBoundColumns );
        hResultSet->hBoundColumns[hResultSet->nBoundColumns - 1] = hBoundColumn;
    }
/********************************************************/

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "SQL_SUCCESS" );
    return SQL_SUCCESS;
}



