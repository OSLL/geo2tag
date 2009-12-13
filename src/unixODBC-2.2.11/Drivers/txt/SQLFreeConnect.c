/**********************************************************************
 * SQLFreeConnect
 *
 * Do not try to Free Dbc if there are Stmts... return an error. Let the
 * Driver Manager do a recursive clean up if its wants.
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

SQLRETURN FreeDbc_( HDRVDBC hDrvDbc )
{
	SQLRETURN	nReturn;

	if ( hDrvDbc == SQL_NULL_HDBC )
		return SQL_ERROR;

	/* TRY TO FREE STATEMENTS						*/
	/* THIS IS JUST IN CASE; SHOULD NOT BE REQUIRED */
	nReturn = FreeStmtList_( hDrvDbc );
	if ( nReturn != SQL_SUCCESS )
		return nReturn;

	/* SPECIAL CHECK FOR FIRST IN LIST 				*/
	if ( ((HDRVENV)hDrvDbc->hEnv)->hFirstDbc == hDrvDbc )
		((HDRVENV)hDrvDbc->hEnv)->hFirstDbc = hDrvDbc->pNext;

	/* SPECIAL CHECK FOR LAST IN LIST 				*/
	if ( ((HDRVENV)hDrvDbc->hEnv)->hLastDbc == hDrvDbc )
		((HDRVENV)hDrvDbc->hEnv)->hLastDbc = hDrvDbc->pPrev;

	/* EXTRACT SELF FROM LIST						*/
	if ( hDrvDbc->pPrev != SQL_NULL_HDBC )
		hDrvDbc->pPrev->pNext = hDrvDbc->pNext;
	if ( hDrvDbc->pNext != SQL_NULL_HDBC )
		hDrvDbc->pNext->pPrev = hDrvDbc->pPrev;

/********************************************************/
/* FREE EXTRAS HERE 						            */
    if ( hDrvDbc->hDbcExtras->pszDatabase )
        free( hDrvDbc->hDbcExtras->pszDatabase );
    if ( hDrvDbc->hDbcExtras->pszDirectory )
        free( hDrvDbc->hDbcExtras->pszDirectory );
    free( hDrvDbc->hDbcExtras );
/********************************************************/

	logClose( hDrvDbc->hLog );
	free( hDrvDbc );

	return SQL_SUCCESS;
}

SQLRETURN SQLFreeConnect_( SQLHDBC hDrvDbc )
{
	HDRVDBC	hDbc	= (HDRVDBC)hDrvDbc;

	/* SANITY CHECKS */
    if ( !hDbc )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hDbc->szSqlMsg, "hDbc = %p", hDbc );
    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hDbc->szSqlMsg );

	if ( hDbc->hFirstStmt )
	{
		logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR Connection has allocated statements" );
		return SQL_ERROR;
	}

	return FreeDbc_( hDbc );

}

SQLRETURN SQLFreeConnect( SQLHDBC hDrvDbc )
{
    return SQLFreeConnect_( hDrvDbc );
}


SQLRETURN FreeDbcList_( HDRVENV hDrvEnv )
{
	if ( hDrvEnv == SQL_NULL_HENV )
		return SQL_SUCCESS;

	while ( FreeDbc_( hDrvEnv->hFirstDbc ) == SQL_SUCCESS )
	{
	}

	return SQL_SUCCESS;
}



