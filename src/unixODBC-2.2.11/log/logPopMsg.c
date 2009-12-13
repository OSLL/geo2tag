/**********************************************************************
 * logPopMsg
 *
 *
 * This code was created by Peter Harvey (mostly during Christmas 98/99).
 * This code is LGPL. Please ensure that this message remains in future
 * distributions and uses of this code (thats about all I get out of it).
 * - Peter Harvey pharvey@codebydesign.com
 *
 **********************************************************************/

#include "log.h"

int logPopMsg( HLOG hLog, char *pszMsgHdr, int *pnCode, char *pszMsg )
{
	char 	*pSeverity;
	HLOGMSG	hMsg;


	if ( !hLog ) return LOG_ERROR;
	if ( !hLog->hMessages ) return LOG_NO_DATA;
	lstLast( hLog->hMessages );
	if ( lstEOL( hLog->hMessages ) ) return LOG_NO_DATA;

	hMsg = lstGet( hLog->hMessages );

	switch ( hMsg->nSeverity )
	{
	case LOG_INFO:
		pSeverity = "INFO:";
		break;
	case LOG_WARNING:
		pSeverity = "WARNING:";
		break;
	case LOG_CRITICAL: 
		pSeverity = "CRITICAL:";
		break;
	default: 
		pSeverity = "UNKNOWN:";
		break;
	}

	sprintf( pszMsgHdr, "[%s][%s][%s] %s ", hLog->pszProgramName, hMsg->pszModuleName, hMsg->pszFunctionName, pSeverity );
	*pnCode = hMsg->nCode;
	sprintf( pszMsg, hMsg->pszMessage );

/*
printf( "--pop-->[%s][%s][%s] %s %s\n", hLog->pszProgramName, hMsg->pszModuleName, hMsg->pszFunctionName, pSeverity, hMsg->pszMessage );
*/
	lstDelete( hLog->hMessages );

	return LOG_SUCCESS;
}


