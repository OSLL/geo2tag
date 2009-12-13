/**********************************************************************
 * logClose
 *
 *
 * This code was created by Peter Harvey (mostly during Christmas 98/99).
 * This code is LGPL. Please ensure that this message remains in future
 * distributions and uses of this code (thats about all I get out of it).
 * - Peter Harvey pharvey@codebydesign.com
 *
 **********************************************************************/

#include "log.h"

int logClose( HLOG hLog )
{
	char szMsgHdr[LOG_MSG_MAX+1];
	int  nCode;
	char szMsg[LOG_MSG_MAX+1];

	if ( !hLog ) return LOG_ERROR;

	/***********************
	 * POP ALL MESSAGES (freeing their mem as the go)
	 ***********************/
	while ( logPopMsg( hLog, szMsgHdr, &nCode, szMsg ) == LOG_SUCCESS )
	{ }

	/***********************
	 * FREE UP SOME MORE MEMORY
	 ***********************/
	if ( hLog->pszProgramName ) free( hLog->pszProgramName );
	if ( hLog->pszLogFile ) free( hLog->pszLogFile );
	lstClose( hLog->hMessages );
	free( hLog );

	return LOG_SUCCESS;
}


