/*****************************************************************************
 * SQLFetchScroll
 *
 **********************************************************************
 *
 * This code was created by Peter Harvey (mostly during Christmas 98/99).
 * This code is LGPL. Please ensure that this message remains in future
 * distributions and uses of this code (thats about all I get out of it).
 * - Peter Harvey pharvey@codebydesign.com
 *
 *****************************************************************************/

#include "driver.h"

SQLRETURN SQLFetchScroll(	SQLHSTMT    hDrvStmt,
							SQLSMALLINT nOrientation,
							SQLROWOFFSET  nOffset )
{
    HDRVSTMT 		hStmt		= (HDRVSTMT)hDrvStmt;

	/* SANITY CHECKS */
	if ( !hStmt )
		return SQL_INVALID_HANDLE;

	sprintf((char*) hStmt->szSqlMsg, "hStmt = %p", hStmt );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hStmt->szSqlMsg );

	/* how much do we want to move */
	switch ( nOrientation )
	{
	case SQL_FETCH_NEXT:
		break;

	case SQL_FETCH_PRIOR:
		break;

	case SQL_FETCH_FIRST:
		break;

	case SQL_FETCH_LAST:
		break;

	case SQL_FETCH_ABSOLUTE:
		break;

	case SQL_FETCH_RELATIVE:
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR: Relative search not currently supported" );
		return SQL_ERROR;
		break;

	case SQL_FETCH_BOOKMARK:
		return SQL_ERROR;
		break;

	default:
		sprintf((char*) hStmt->szSqlMsg, "SQL_ERROR: Unknown fetch orientation (%d)", nOrientation );
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hStmt->szSqlMsg );
		return SQL_ERROR;
	}

    /* HANDLE BOUND COLUMNS HERE */

	logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "SQL_SUCCESS" );
    return SQL_SUCCESS;
}


