/**********************************************************************
 * SQLAllocStmt (deprecated)
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

SQLRETURN SQLAllocStmt_(   SQLHDBC     hDrvDbc,
                          SQLHSTMT    *phDrvStmt )
{
    HDRVDBC     hDbc	= (HDRVDBC)hDrvDbc;
	HDRVSTMT    *phStmt	= (HDRVSTMT*)phDrvStmt;

    /* SANITY CHECKS */
    if ( !hDbc )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hDbc->szSqlMsg, "START: hDbc = %p", hDbc );
    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hDbc->szSqlMsg );

    if ( !phStmt )
    {
        logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: phStmt=NULL" );
        return SQL_ERROR;
    }

    /* allocate memory */
    *phStmt = malloc( sizeof(DRVSTMT) );
    if( SQL_NULL_HSTMT == *phStmt )
    {
        logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_CRITICAL, LOG_CRITICAL, "END: memory allocation failure" );
        return SQL_ERROR;
    }

    /* initialize memory */
	sprintf((char*) hDbc->szSqlMsg, "*phstmt = %p", *phStmt );
    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hDbc->szSqlMsg );

	memset( *phStmt, 0, sizeof(DRVSTMT) );	/* SAFETY */
    (*phStmt)->hDbc			    = (SQLPOINTER)hDbc;
    (*phStmt)->hLog			    = NULL;
    (*phStmt)->hStmtExtras	    = NULL;
    (*phStmt)->pNext		    = NULL;
    (*phStmt)->pPrev		    = NULL;
    (*phStmt)->pszQuery		    = NULL;
    (*phStmt)->pRowStatusArray  = NULL;
    sprintf((char*)(*phStmt)->szCursorName, "CUR_%p", *phStmt );

	/* ADD TO DBCs STATEMENT LIST */
	
	/* start logging		*/
    if ( logOpen( &(*phStmt)->hLog, "odbctxt", hDbc->hLog->pszLogFile, 50 ) )
		logOn( (*phStmt)->hLog, 1 );
	else
		(*phStmt)->hLog = NULL;

	/* ADD TO END OF LIST */
	if ( hDbc->hFirstStmt == NULL )
	{
		/* 1st is null so the list is empty right now */
        hDbc->hFirstStmt		= (*phStmt);
        hDbc->hLastStmt			= (*phStmt);
	}
	else
	{
		/* at least one node in list */
		hDbc->hLastStmt->pNext	= (SQLPOINTER)(*phStmt);
		(*phStmt)->pPrev		= (SQLPOINTER)hDbc->hLastStmt;
        hDbc->hLastStmt			= (*phStmt);
	}

/********************************************************/
/* DRIVER SPECIFIC               						*/

    (*phStmt)->hStmtExtras      = calloc( 1, sizeof(STMTEXTRAS));

/****************************************************************************/

    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success." );

    return SQL_SUCCESS;
}


SQLRETURN SQLAllocStmt(   SQLHDBC     hDrvDbc,
                          SQLHSTMT    *phDrvStmt )
{
    return SQLAllocStmt_( hDrvDbc, phDrvStmt );
}



