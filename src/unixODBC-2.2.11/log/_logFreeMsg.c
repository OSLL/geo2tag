/**********************************************************************
 * _logFreeMsg
 *
 * This code was created by Peter Harvey (mostly during Christmas 98/99).
 * This code is LGPL. Please ensure that this message remains in future
 * distributions and uses of this code (thats about all I get out of it).
 * - Peter Harvey pharvey@codebydesign.com
 *
 **********************************************************************/

#include "log.h"

void _logFreeMsg( void *pMsg )
{
	HLOGMSG hMsg	= (HLOGMSG)pMsg;

	if ( !hMsg ) return;

	/* free msg memory */
	if ( hMsg->pszModuleName != NULL ) free(hMsg->pszModuleName);
	if ( hMsg->pszFunctionName != NULL ) free(hMsg->pszFunctionName);
	if ( hMsg->pszMessage != NULL ) free(hMsg->pszMessage);

	free( hMsg );
}


