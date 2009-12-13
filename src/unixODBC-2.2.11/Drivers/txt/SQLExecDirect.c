/**********************************************************************
 * SQLExecDirect
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

SQLRETURN SQLExecDirect(  SQLHSTMT    hDrvStmt,
						  SQLCHAR     *szSqlStr,
						  SQLINTEGER  nSqlStr )
{
    HDRVSTMT 	hStmt	= (HDRVSTMT)hDrvStmt;
	SQLRETURN	nReturn;
					
	/* SANITY CHECKS */
    if( NULL == hStmt )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hStmt->szSqlMsg, "hStmt = %p", hStmt );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hStmt->szSqlMsg );

	/* prepare command */
	nReturn = SQLPrepare_( hDrvStmt, szSqlStr, nSqlStr );
	if ( nReturn != SQL_SUCCESS )
	{
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "Could not prepare statement" );
		return nReturn;
	}

	/* execute command */
	nReturn = SQLExecute_( hDrvStmt );
	if ( nReturn != SQL_SUCCESS )
	{
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "Problem calling SQLEXecute" );
		return nReturn;
	}

	logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "SQL_SUCCESS" );
	return SQL_SUCCESS;
}


