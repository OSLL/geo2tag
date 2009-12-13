/**********************************************************************
 * logOpen
 *
 *
 * This code was created by Peter Harvey (mostly during Christmas 98/99).
 * This code is LGPL. Please ensure that this message remains in future
 * distributions and uses of this code (thats about all I get out of it).
 * - Peter Harvey pharvey@codebydesign.com
 *
 **********************************************************************/

#include "log.h"

int logOpen( HLOG *phLog, char *pszProgramName, char *pszLogFile, long nMaxMsgs )
{

	/* LOG STRUCT */
	*phLog = malloc( sizeof(LOG) );
    (*phLog)->nMaxMsgs			= nMaxMsgs;
    (*phLog)->hMessages			= lstOpen();
    (*phLog)->bOn				= 0;
    (*phLog)->pszLogFile		= NULL;
    (*phLog)->pszProgramName	= NULL;

	lstSetFreeFunc( (*phLog)->hMessages, _logFreeMsg );

	/* PROGRAM NAME */
	if ( pszProgramName )
		(*phLog)->pszProgramName = (char *)strdup( pszProgramName );
	else
		(*phLog)->pszProgramName = (char *)strdup( "UNKNOWN" );

	/* LOG FILE */
	if ( pszLogFile )
		(*phLog)->pszLogFile = (char*)strdup( pszLogFile );

	return LOG_SUCCESS;
}


