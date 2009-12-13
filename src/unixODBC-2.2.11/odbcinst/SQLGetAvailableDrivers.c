/**************************************************
 * SQLGetAvailableDrivers
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under LGPL 28.JAN.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/
#include <odbcinstext.h>

BOOL SQLGetAvailableDrivers(	LPCSTR     	pszInfFile,
								LPSTR      	pszBuf,
								WORD       	nBufMax,
								WORD 		*pnBufOut)
{
	return SQLGetInstalledDrivers(	pszBuf,	nBufMax, pnBufOut );
}


