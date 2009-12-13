/**********************************************************************
 * SQLDisconnect
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

SQLRETURN SQLDisconnect( SQLHDBC hDrvDbc )
{
	HDRVDBC hDbc	= (HDRVDBC)hDrvDbc;

	/* SANITY CHECKS */
    if( !hDbc )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hDbc->szSqlMsg, "hDbc = %p", hDbc );
    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, (char*)hDbc->szSqlMsg );

    if( hDbc->bConnected == 0 )
    {
        logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_SUCCESS_WITH_INFO Connection not open" );
        return SQL_SUCCESS_WITH_INFO;
    }

	if ( hDbc->hFirstStmt )     /* A app is allowed to disconnect with existing statements, we have to */
	{                           /* live with it */
        FreeStmtList_( hDbc );
	}

/********************************************************/
/* DRIVER SPECIFIC               						*/
    if ( hDbc->hDbcExtras->pszDatabase )
        free( hDbc->hDbcExtras->pszDatabase );
    if ( hDbc->hDbcExtras->pszDirectory )
        free( hDbc->hDbcExtras->pszDirectory );
    hDbc->hDbcExtras->cColumnSeperator  = DEFAULTCOLUMNSEPERATOR;
    hDbc->hDbcExtras->pszDatabase       = NULL;
    hDbc->hDbcExtras->pszDirectory      = NULL;
/********************************************************/

	if ( hDbc->hLog->pszLogFile )
		free( hDbc->hLog->pszLogFile );

    hDbc->hLog->pszLogFile			= NULL;
    hDbc->bConnected 				= 0;

    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "SQL_SUCCESS" );

    return SQL_SUCCESS;
}


