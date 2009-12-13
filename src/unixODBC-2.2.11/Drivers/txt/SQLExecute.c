/**********************************************************************
 * SQLExecute
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


SQLRETURN SQLExecute_( SQLHSTMT  hDrvStmt )
{
    HDRVSTMT 	hStmt	= (HDRVSTMT)hDrvStmt;

	/* SANITY CHECKS */
    if( !hStmt )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hStmt->szSqlMsg, "hStmt = %p", hStmt );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hStmt->szSqlMsg );

    if( !hStmt->pszQuery )
    {
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR No statement" );
        return SQL_ERROR;
    }

    hStmt->nRowsAffected = -1;

/********************************************************/
/* DRIVER SPECIFIC               						*/
    if ( !hStmt->hStmtExtras->hParsedSQL )
    {
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR No prepared statement" );
        return SQL_ERROR;
    }

    FreeResultSet_( hStmt->hStmtExtras );
    
    switch ( hStmt->hStmtExtras->hParsedSQL->nType )
    {
    case sqpcreatetable:
        if ( SQL_SUCCESS != IOCreateTable( hStmt ) )
        {
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR CREATE TABLE failed." );
            return SQL_ERROR;
        }
        break;
    case sqpdroptable:
        if ( SQL_SUCCESS != IODropTable( hStmt ) )
        {
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR DROP TABLE failed." );
            return SQL_ERROR;
        }
        break;
    case sqpselect:
        if ( SQL_SUCCESS != IOSelectTable( hStmt ) )
        {
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR SELECT failed." );
            return SQL_ERROR;
        }
        break;
    case sqpinsert:
        if ( SQL_SUCCESS != IOInsertTable( hStmt ) )
        {
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR INSERT failed." );
            return SQL_ERROR;
        }
        break;
    case sqpdelete:
        if ( SQL_SUCCESS != IODeleteTable( hStmt ) )
        {
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR DELETE failed." );
            return SQL_ERROR;
        }
        break;
    case sqpupdate:
        if ( SQL_SUCCESS != IOUpdateTable( hStmt ) )
        {
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR UPDATE failed." );
            return SQL_ERROR;
        }
        break;
    default:
        logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR SQL statement not supported." );
        return SQL_ERROR;
    }
/********************************************************/

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "SQL_SUCCESS" );
    return SQL_SUCCESS;
}


SQLRETURN SQLExecute( SQLHSTMT  hDrvStmt )
{
    return SQLExecute_( hDrvStmt );
}




