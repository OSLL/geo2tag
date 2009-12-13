/**********************************************************************
 * SQLPrepare
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

SQLRETURN SQLPrepare_(  SQLHSTMT    hDrvStmt,
                        SQLCHAR     *szSqlStr,
                        SQLINTEGER  nSqlStrLength )
{   
    HDRVSTMT    hStmt       = (HDRVSTMT)hDrvStmt;


    /* SANITY CHECKS */
    if ( !hStmt )
        return SQL_INVALID_HANDLE;

    sprintf((char*) hStmt->szSqlMsg, "hStmt = %p", hStmt );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hStmt->szSqlMsg );

    if ( !szSqlStr )
    {
        logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR No SQL to process" );
        return SQL_ERROR;
    }

/* PAH - Long messages seem to cause things to seg fault so lets not echo back the sql statement.
 *
 *	logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, szSqlStr );
 */

/********************************************************/
/* DRIVER SPECIFIC               						*/
    nSqlStrLength = 0; /* GET RID OF COMPILE WARNING MESSAGE */
    ResetStmt_( hStmt );

    /* PAH - We want to append a space to the sql statement because of a problem
     *       in the lexer when the last char is a closing quote.
     */


    hStmt->pszQuery = calloc( 1, (strlen((char*)szSqlStr)+10) );
    strcpy((char*) hStmt->pszQuery, (char*)szSqlStr );
    hStmt->pszQuery[strlen((char*)szSqlStr)] = ' ';


    sqpOpen((char*) hStmt->pszQuery, (char*)&(hStmt->pszQuery[strlen((char*)hStmt->pszQuery)]), hStmt->hStmtExtras->hParams );

    if ( sqpParse() )
    {
        logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, sqpError() );
        sqpClose();
        free( hStmt->pszQuery );
        hStmt->pszQuery = NULL;
        return SQL_ERROR;
    }
    hStmt->hStmtExtras->hParsedSQL = sqpAdoptParsedSQL();

    sqpClose();
/********************************************************/

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "SQL_SUCCESS" );
    return SQL_SUCCESS;
}




SQLRETURN SQLPrepare(   SQLHSTMT    hDrvStmt,
                        SQLCHAR     *szSqlStr,
                        SQLINTEGER  nSqlStrLength )
{   
    return SQLPrepare_( hDrvStmt, szSqlStr, nSqlStrLength );
}


