/**************************************************
 * 
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign. 
 * Released under GPL 31.JAN.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/

#include <stdio.h>
#include <ltdl.h>
#include "CDLL.h"

CDLL::CDLL( char *szFileName )
{
	szError[0] = '\0';

    /*
     * initialize libtool
     */

    lt_dlinit();

	hDLL = lt_dlopen( szFileName );
	if ( !hDLL )
		strncpy( szError, lt_dlerror(), 200 );
}


CDLL::~CDLL()
{
	if ( hDLL )
		lt_dlclose( hDLL );
}

int CDLL::Symbol( HCBDPROC *hProc, char *szSymbol )
{
	const char 		*pError;

	*hProc = (HCBDPROC) lt_dlsym( hDLL, szSymbol );

	pError = lt_dlerror();
	if ( pError )
	{
		strncpy( szError, pError, 200 );
		return 0;
	}

	return 1;
}
