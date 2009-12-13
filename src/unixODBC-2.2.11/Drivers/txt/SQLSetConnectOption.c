/**********************************************************************
 * SQLSetConnectOption (deprecated)
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

SQLRETURN SQLSetConnectOption(  SQLHDBC hDrvDbc,
								UWORD   nOption,
								SQLULEN  vParam)
{
	HDRVDBC	hDbc	= (HDRVDBC)hDrvDbc;

	/* SANITY CHECKS */
	if ( hDbc == SQL_NULL_HDBC )
		return SQL_INVALID_HANDLE;

	sprintf((char*) hDbc->szSqlMsg, "START: hDbc = %p nOption = %d", hDbc, nOption );
    logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hDbc->szSqlMsg );

/********************************************************/
/* DRIVER SPECIFIC               						*/
	switch ( nOption )
	{
    case SQL_LOGIN_TIMEOUT:
        break;
	case SQL_TRANSLATE_DLL:
	case SQL_TRANSLATE_OPTION:
/*	case SQL_CONNECT_OPT_DRVR_START: */
	case SQL_ODBC_CURSORS:
	case SQL_OPT_TRACE:
		switch ( vParam )
		{
		case SQL_OPT_TRACE_ON:
		case SQL_OPT_TRACE_OFF:
			default:
			;
		}
		break;

	case SQL_OPT_TRACEFILE:
	case SQL_ACCESS_MODE:
	case SQL_AUTOCOMMIT:
	default:
        logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Option not supported." );
        return SQL_ERROR;
	}
/********************************************************/

	logPushMsg( hDbc->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success." );
	return SQL_SUCCESS;
}


