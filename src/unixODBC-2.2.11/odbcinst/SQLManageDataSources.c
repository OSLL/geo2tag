/**************************************************
 * SQLManageDataSources
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

BOOL SQLManageDataSources( HWND hWnd )
{
	BOOL	nReturn;
	char    szGUILibFile[FILENAME_MAX];
	void 	*hDLL;
	BOOL	(*pSQLManageDataSources)( HWND	);
    HODBCINSTWND    hODBCINSTWnd;

    /*
     * SANITY CHECKS
     *
     */
	if ( !hWnd )
	{
        inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_INVALID_HWND, "No hWnd" );
		return FALSE;
	}
    hODBCINSTWnd = (HODBCINSTWND)hWnd;
	if ( !hODBCINSTWnd->hWnd )
	{
        inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_INVALID_HWND, "No hODBCINSTWnd->hWnd" );
		return FALSE;
	}

    /*
     * 
     *
     */
    if ( lt_dlinit() )
    {
        inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_GENERAL_ERR, "lt_dlinit() failed" );
		return FALSE;
    }

    /*
     * DETERMINE PATH FOR GUI PLUGIN
     *
     */
    if ( strncasecmp( hODBCINSTWnd->szGUI, "QT", 2 ) == 0 )
    {
        char *p;
/* 
 * This works and extension 'la' is probably more portable but we lose 
 * control of which version of the lib we load (it hard coded in the *.la)
 *
 *        sprintf( szGUILibFile, "%s/libodbcinstQ.la", DEFLIB_PATH );
 *
 * Bring up libodbcinstQ.la in a text editor and you will see the name we 
 * are supposed to use to dlopen the lib, worse than the above option
 *
 *        sprintf( szGUILibFile, "%s/libodbcinstQ.so.1", DEFLIB_PATH );
 *
 * This is less portable because of 'so' extension but we can let the file 
 * symlinks work for us
 *
 *        sprintf( szGUILibFile, "%s/libodbcinstQ.so", DEFLIB_PATH );
 */

        /*
         * first look in the environment
         */

        p  = getenv( "ODBCINSTQ" );
        if ( p )
        {
            strcpy( szGUILibFile, p );
        }
        else
        {
            SQLGetPrivateProfileString( "ODBC", "ODBCINSTQ", "", szGUILibFile, sizeof( szGUILibFile ), "odbcinst.ini" );

            if ( strlen( szGUILibFile ) == 0 )
            {
                /*
                 * we need to find the extension to use as well
                 */

#ifdef SHLIBEXT
                if ( strlen( SHLIBEXT ) > 0 )
                    sprintf( szGUILibFile, "libodbcinstQ%s.1", SHLIBEXT );
                else
                    sprintf( szGUILibFile, "libodbcinstQ.so.1" );
#else
                sprintf( szGUILibFile, "libodbcinstQ.so.1" );
#endif        

                if ( lt_dladdsearchdir( DEFLIB_PATH ) )
                {
                    inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, 
                            ODBC_ERROR_GENERAL_ERR, (char*)lt_dlerror() );
                }
            }
        }
    }
    else
    {
        inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, 
                ODBC_ERROR_INVALID_HWND, "Unsupported hODBCINSTWnd->szGUI" );
		return FALSE;
    }

    /*
     * USE libtool TO LOAD PLUGIN AND CALL FUNCTION
     *
     */
    nReturn = FALSE;
    hDLL = lt_dlopen( szGUILibFile );
	if ( hDLL )
	{
        /* change the name, as it avoids it finding it in the calling lib */
		pSQLManageDataSources = (BOOL (*)(HWND))lt_dlsym( hDLL, "QTSQLManageDataSources" );
		if ( pSQLManageDataSources )
			nReturn = pSQLManageDataSources( (HWND)hODBCINSTWnd->hWnd );
		else
			inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_GENERAL_ERR, (char*)lt_dlerror() );
	}
	else
    {
        /*
         * try adding szGUILibFile
         */

        sprintf( szGUILibFile, "%s/libodbcinstQ%s.1", DEFLIB_PATH, SHLIBEXT );
        hDLL = lt_dlopen( szGUILibFile );
        if ( hDLL )
        {
            /* change the name, as it avoids it finding it in the calling lib */
            pSQLManageDataSources = (BOOL (*)(HWND))lt_dlsym( hDLL, "QTSQLManageDataSources" );
            if ( pSQLManageDataSources )
                nReturn = pSQLManageDataSources( (HWND)hODBCINSTWnd->hWnd );
            else
                inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_GENERAL_ERR, (char*)lt_dlerror() );
        }

		inst_logPushMsg( __FILE__, __FILE__, __LINE__, LOG_CRITICAL, ODBC_ERROR_GENERAL_ERR, (char*)lt_dlerror() );
    }

	return nReturn;
}
