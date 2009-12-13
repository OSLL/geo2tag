/**************************************************
 * SQLCreateDataSource
 *
 * This 100% GUI so simply pass it on to odbcinst's GUI
 * shadow share.
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

BOOL SQLCreateDataSource(		HWND	hWnd,
								LPCSTR	pszDS )
{
	BOOL	nReturn;
	void 	*hDLL;
	BOOL	(*pSQLCreateDataSource)( HWND, LPSTR	);

	if ( !hWnd )
	{
        inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_INVALID_HWND, "" );
		return FALSE;
	}

    /*
     * initialize libtool
     */

    lt_dlinit();

	nReturn = FALSE;
	/* TRY TO PASS THROUGH TO GUI SHADOW LIB */
	if ( (hDLL = lt_dlopen( "libodbcinstG.so" ))  )
	{
		pSQLCreateDataSource = (BOOL(*)(HWND,LPSTR))lt_dlsym( hDLL, "SQLCreateDataSource" );
		if ( pSQLCreateDataSource )
			nReturn = pSQLCreateDataSource( hWnd, (char *)pszDS );
		else
			inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_GENERAL_ERR, "" );
	}
	else
		inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_GENERAL_ERR, "" );

	return nReturn;
}


