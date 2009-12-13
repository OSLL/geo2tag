/**********************************************************************
 * logPushMsg
 *
 *
 * This code was created by Peter Harvey (mostly during Christmas 98/99).
 * This code is LGPL. Please ensure that this message remains in future
 * distributions and uses of this code (thats about all I get out of it).
 * - Peter Harvey pharvey@codebydesign.com
 *
 **********************************************************************/

#include "log.h"
#include "ini.h"

int logPushMsg( HLOG hLog, char *pszModule, char *pszFunctionName, int nLine, int nSeverity, int nCode, char *pszMessage )
{
    HLOGMSG		hMsg;
	FILE 		*hFile;

	if ( !hLog ) return LOG_ERROR;
	if ( !hLog->hMessages ) return LOG_ERROR;
	if ( !hLog->bOn ) return LOG_SUCCESS;

	if ( !pszModule ) return LOG_ERROR;
	if ( !pszFunctionName ) return LOG_ERROR;
	if ( !pszMessage ) return LOG_ERROR;


	/* check for, and handle, max msg */
	if ( hLog->hMessages->nItems == hLog->nMaxMsgs )
	{
		lstFirst( hLog->hMessages );
        lstDelete( hLog->hMessages );
	}

	/* allocate msg */
	hMsg 					= malloc( sizeof(LOGMSG) );
	hMsg->pszModuleName		= (char *)strdup( pszModule );
	hMsg->pszFunctionName	= (char *)strdup( pszFunctionName );
	hMsg->pszMessage		= (char *)strdup( pszMessage );
	hMsg->nLine				= nLine;
	hMsg->nSeverity			= nSeverity;
	hMsg->nCode				= nCode;

	/* append to  list */
	lstAppend( hLog->hMessages, hMsg );

	/* append to file */
	if ( hLog->pszLogFile )
	{
		hFile = uo_fopen( hLog->pszLogFile, "a" );
		if ( !hFile ) return LOG_ERROR;

		uo_fprintf( hFile, "[%s][%s][%s][%d]%s\n", hLog->pszProgramName, hMsg->pszModuleName, hMsg->pszFunctionName, hMsg->nLine, hMsg->pszMessage );

		uo_fclose( hFile );
	}

	return LOG_SUCCESS;
}


