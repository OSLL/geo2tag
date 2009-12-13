/**************************************************
 * SQLConfigDriver
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

BOOL SQLConfigDriver(           HWND	hWnd,
								WORD	nRequest,
								LPCSTR	pszDriver,				/* USER FRIENDLY NAME (not file name) */
								LPCSTR	pszArgs,
								LPSTR	pszMsg,
								WORD	nMsgMax,
								WORD	*pnMsgOut )
{
	BOOL	nReturn;
	void 	*hDLL;
	BOOL	(*pConfigDriver)( HWND, WORD, LPCSTR, LPCSTR, LPCSTR, WORD, WORD *	);
	char	szDriverSetup[ODBC_FILENAME_MAX+1];
	HINI	hIni;
    char    szIniName[ INI_MAX_OBJECT_NAME + 1 ];


	/* SANITY CHECKS */
    nReturn = FALSE;
	if ( pszDriver == NULL )
	{
		inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_INVALID_NAME, "" );
		return FALSE;
	}
	if ( nRequest < ODBC_CONFIG_DRIVER )
	{
		inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_INVALID_REQUEST_TYPE, "" );
		return FALSE;
	}

	/* OK */

#ifdef VMS
    sprintf( szIniName, "%sODBCINST.INI", odbcinst_system_file_path() );
#else
    sprintf( szIniName, "%s/odbcinst.ini", odbcinst_system_file_path() );
#endif

	/* lets get driver setup file name from odbcinst.ini */
#ifdef __OS2__
	if ( iniOpen( &hIni, szIniName, "#;", '[', ']', '=', TRUE, 1L ) != INI_SUCCESS )
#else
	if ( iniOpen( &hIni, szIniName, "#;", '[', ']', '=', TRUE ) != INI_SUCCESS )
#endif
	{
		inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_INVALID_NAME, "" );
		return FALSE;
	}

#ifdef PLATFORM64
	if ( iniPropertySeek( hIni, (char *)pszDriver, "Setup64", "" ) == INI_SUCCESS || 
				iniPropertySeek( hIni, (char *)pszDriver, "Setup", "" ) == INI_SUCCESS )
#else
	if ( iniPropertySeek( hIni, (char *)pszDriver, "Setup", "" ) != INI_SUCCESS )
#endif
	{
		inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_INVALID_NAME, "" );
		iniClose( hIni );
		return FALSE;
	}
	iniValue( hIni, szDriverSetup );
	iniClose( hIni );

    /*
     * initialize libtool
     */

    lt_dlinit();

	/* process request */
	switch ( nRequest )
	{
	case ODBC_CONFIG_DRIVER:
		/* WHAT OPTIONS CAN WE EXPECT IN pszArgs??
		 * Sounds like just connection pooling options
		 * In anycase, the spec says handle this in the
		 * odbcinst so we probably want to make some calls here...
		 * How common are Driver config options (not DSN options) anyway?
		 * - Peter
		 */
		break;
	case ODBC_INSTALL_DRIVER:
	case ODBC_REMOVE_DRIVER:
	default :						/* DRIVER SEPCIFIC are default; HANDLE AS PER INSTALL & REMOVE */
		/* errors in here are ignored, according to the spec; perhaps I should ret error and let app ignore? */
		if ( (hDLL = lt_dlopen( szDriverSetup ))  )
		{
			pConfigDriver = (BOOL (*)(HWND, WORD, LPCSTR, LPCSTR, LPCSTR, WORD, WORD * )) lt_dlsym( hDLL, "ConfigDriver" );
/*			if ( lt_dlerror() == NULL ) */
            if ( pConfigDriver )
				nReturn = pConfigDriver( hWnd, nRequest, pszDriver, pszArgs, pszMsg, nMsgMax, pnMsgOut);
			else
				inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_GENERAL_ERR, "" );
		}
		else
			inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_GENERAL_ERR, "" );
	}

	return TRUE;
}


