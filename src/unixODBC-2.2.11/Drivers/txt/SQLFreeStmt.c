/**********************************************************************
 * SQLFreeStmt
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

void ResetStmt_( HDRVSTMT hStmt )
{
    if ( hStmt->hStmtExtras->hResultSet )
        FreeResultSet_( hStmt->hStmtExtras );
    if ( hStmt->hStmtExtras->hParsedSQL )
        sqpFreeParsedSQL( hStmt->hStmtExtras->hParsedSQL );
    if ( hStmt->pszQuery )
        free( hStmt->pszQuery );

    hStmt->hStmtExtras->hParsedSQL  = NULL;
    hStmt->pszQuery                 = NULL;
    hStmt->nRowsAffected            = -1;
}

SQLRETURN FreeBoundCols_( HSTMTEXTRAS hStmt )
{
    HRESULTSET      hResultSet;
    HBOUNDCOLUMNS   hBoundColumns;

	if ( !hStmt )
		return SQL_ERROR;

    if ( !hStmt->hResultSet )
        return SQL_SUCCESS;

    hResultSet = hStmt->hResultSet;

    /* FREE BOUND COLUMNS */
    hBoundColumns = hResultSet->hBoundColumns;
    for ( hResultSet->nBoundColumn = 1; hResultSet->nBoundColumn <= hResultSet->nBoundColumns; hResultSet->nBoundColumn++ )
    {
        free( hBoundColumns[hResultSet->nBoundColumn - 1] );
    }
    free( hBoundColumns );
    hResultSet->nBoundColumns = 0;
    hBoundColumns = hResultSet->hBoundColumns = NULL;

    return SQL_SUCCESS;
}

SQLRETURN FreeResultSet_( HSTMTEXTRAS hStmt )
{
    HRESULTSET      hResultSet;
    HBOUNDCOLUMNS   hBoundColumns;
    HCOLUMNS        hColumns;
    HCOLUMN         hColumn;
    HROWS           hRows;
    HROW            hRow;

	if ( !hStmt )
		return SQL_ERROR;

    if ( !hStmt->hResultSet )
        return SQL_SUCCESS;

    FreeBoundCols_( hStmt );

    hResultSet = hStmt->hResultSet;

    /* FREE COLUMNS */
    FreeColumns_( &hResultSet->hColumns, hResultSet->nCols );

    /* FREE ROWS */
    FreeRows_( &hResultSet->hRows, hResultSet->nRows, hResultSet->nCols );

    /* FREE CONTAINER */
    free( hStmt->hResultSet );
    hStmt->hResultSet   = NULL;

	return SQL_SUCCESS;
}

SQLRETURN FreeParams_( HSTMTEXTRAS hStmt )
{
	if ( !hStmt )
		return SQL_ERROR;

    if ( !hStmt->hParams )
        return SQL_SUCCESS;

    lstSetFreeFunc( hStmt->hParams, sqpFreeParam );
    lstClose( hStmt->hParams );

    hStmt->hParams   = NULL;

	return SQL_SUCCESS;
}

SQLRETURN FreeStmt_( HDRVSTMT hDrvStmt )
{
	if ( hDrvStmt == SQL_NULL_HSTMT )
		return SQL_ERROR;

	sprintf((char*) hDrvStmt->szSqlMsg, "hStmt (%p)", hDrvStmt );
    logPushMsg( hDrvStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hDrvStmt->szSqlMsg );

	/* SPECIAL CHECK FOR FIRST IN LIST 				*/
	if ( ((HDRVDBC)hDrvStmt->hDbc)->hFirstStmt == hDrvStmt )
		((HDRVDBC)hDrvStmt->hDbc)->hFirstStmt = hDrvStmt->pNext;
	
	/* SPECIAL CHECK FOR LAST IN LIST 				*/
	if ( ((HDRVDBC)hDrvStmt->hDbc)->hLastStmt == hDrvStmt )
		((HDRVDBC)hDrvStmt->hDbc)->hLastStmt = hDrvStmt->pPrev;

	/* EXTRACT SELF FROM LIST						*/
	if ( hDrvStmt->pPrev != SQL_NULL_HSTMT )
		hDrvStmt->pPrev->pNext = hDrvStmt->pNext;
	if ( hDrvStmt->pNext != SQL_NULL_HSTMT )
		hDrvStmt->pNext->pPrev = hDrvStmt->pPrev;

	/* FREE MEMORY */
	if ( hDrvStmt->pszQuery ) 
        free( hDrvStmt->pszQuery );

/********************************************************/
/* FREE EXTRAS HERE 						            */
    FreeResultSet_( hDrvStmt->hStmtExtras );
    FreeParams_( hDrvStmt->hStmtExtras );
    if ( hDrvStmt->hStmtExtras->hParsedSQL )
        sqpFreeParsedSQL( hDrvStmt->hStmtExtras->hParsedSQL );
	free( hDrvStmt->hStmtExtras );
/********************************************************/

	logClose( hDrvStmt->hLog );
	free( hDrvStmt );

	return SQL_SUCCESS;
}


SQLRETURN SQLFreeStmt_(	SQLHSTMT        hDrvStmt,
						SQLUSMALLINT    nOption )
{
    HDRVSTMT hStmt	= (HDRVSTMT)hDrvStmt;

	/* SANITY CHECKS */
    if( hStmt == SQL_NULL_HSTMT )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hStmt->szSqlMsg, "START: hStmt = %p nOption = %d", hStmt, nOption );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );

/********************************************************/
/* DRIVER SPECIFIC               						*/
    switch( nOption )
    {
	case SQL_CLOSE:
        return FreeResultSet_( hStmt->hStmtExtras );

    case SQL_DROP:
        return FreeStmt_( hStmt );

    case SQL_RESET_PARAMS:
        return FreeParams_( hStmt->hStmtExtras );
        break;

	case SQL_UNBIND:
        return FreeBoundCols_( hStmt->hStmtExtras );

    default:
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Unsupported option." );
        return SQL_ERROR;
    }
/********************************************************/

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success." );
    return SQL_SUCCESS;
}


SQLRETURN SQLFreeStmt(	SQLHSTMT        hDrvStmt,
						SQLUSMALLINT    nOption )
{
    return SQLFreeStmt_( hDrvStmt, nOption );
}

SQLRETURN FreeStmtList_( HDRVDBC hDrvDbc )
{
	if ( hDrvDbc == SQL_NULL_HDBC )
		return SQL_SUCCESS;

	if ( hDrvDbc->hFirstStmt == NULL )
		return SQL_SUCCESS;
	
	while ( FreeStmt_( hDrvDbc->hFirstStmt ) == SQL_SUCCESS )
	{
	}

	return SQL_SUCCESS;
}



