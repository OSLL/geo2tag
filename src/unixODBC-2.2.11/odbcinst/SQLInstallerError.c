/**************************************************
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

typedef struct tODBCINSTErrorMsg
{
	int 	nCode;
	char	*szMsg;
} ODBCINSTErrorMsg;

static ODBCINSTErrorMsg aODBCINSTErrorMsgs[] =
{
    0,                                      "Filler",
	ODBC_ERROR_GENERAL_ERR,					"General installer error",
	ODBC_ERROR_INVALID_BUFF_LEN,			"Invalid buffer length",
	ODBC_ERROR_INVALID_HWND,				"Invalid window handle",
	ODBC_ERROR_INVALID_STR,					"Invalid string",
	ODBC_ERROR_INVALID_REQUEST_TYPE,		"Invalid type of request",
	ODBC_ERROR_COMPONENT_NOT_FOUND,			"Unable to find component name",
	ODBC_ERROR_INVALID_NAME,				"Invalid driver or translator name",
	ODBC_ERROR_INVALID_KEYWORD_VALUE,		"Invalid keyword-value pairs",
	ODBC_ERROR_INVALID_DSN,					"Invalid DSN",
	ODBC_ERROR_INVALID_INF,					"Invalid INF",
	ODBC_ERROR_REQUEST_FAILED,				"General error request failed",
	ODBC_ERROR_INVALID_PATH,				"Invalid install path",
	ODBC_ERROR_LOAD_LIB_FAILED,				"Could not load the driver or translator setup library",
	ODBC_ERROR_INVALID_PARAM_SEQUENCE,		"Invalid parameter sequence",
	ODBC_ERROR_INVALID_LOG_FILE,			"INvalid log file",
	ODBC_ERROR_USER_CANCELED,				"User canceled operation",
	ODBC_ERROR_USAGE_UPDATE_FAILED,			"Could not increment or decrement the component usage count",
	ODBC_ERROR_CREATE_DSN_FAILED,			"Could not create the requested DSN",
	ODBC_ERROR_WRITING_SYSINFO_FAILED,		"Error writing sysinfo",
	ODBC_ERROR_REMOVE_DSN_FAILED,			"Removing DSN failed",
	ODBC_ERROR_OUT_OF_MEM,					"Out of memory",
	ODBC_ERROR_OUTPUT_STRING_TRUNCATED,		"String right truncated"
};


/* THIS WILL HOLD OUR LOG MESSAGES. ITS INIT IN _init() AND MEMORY IS FREED IN _fini() */
/* HLOG hODBCINSTLog; */

RETCODE SQLInstallerError(		WORD	nError,
								DWORD	*pnErrorCode,
								LPSTR	pszErrorMsg,
								WORD	nErrorMsgMax,
								WORD	*pnErrorMsg )
{
	char szMsgHdr[LOG_MSG_MAX+1];
	char szMsg[LOG_MSG_MAX+1];

	if ( nError != 1 || pnErrorCode == NULL || pszErrorMsg == NULL )
		return SQL_NO_DATA;

    pszErrorMsg[0] = '\0';

	if ( inst_logPopMsg( szMsgHdr, (int *)pnErrorCode, szMsg ) != LOG_SUCCESS )
		return SQL_NO_DATA;

	/*
     * change this to return the same text as windows
     *
    sprintf( pszErrorMsg, "%s\n%s\n%s", szMsgHdr, (aODBCINSTErrorMsgs[*pnErrorCode]).szMsg, szMsg );

	if ( pnErrorMsg != NULL )
		*pnErrorMsg = strlen( pszErrorMsg );

     */

    if ( pnErrorMsg )
    {
        *pnErrorMsg = strlen( (aODBCINSTErrorMsgs[*pnErrorCode]).szMsg );
    }

    if ( strlen( (aODBCINSTErrorMsgs[*pnErrorCode]).szMsg ) > nErrorMsgMax )
    {
        if ( pszErrorMsg )
        {
            strncpy( pszErrorMsg, (aODBCINSTErrorMsgs[*pnErrorCode]).szMsg, nErrorMsgMax );
            pnErrorCode[ nErrorMsgMax ] = '\0';
        }
        return SQL_SUCCESS_WITH_INFO;
    }
    else
    {
        if ( pszErrorMsg )
        {
            strcpy( pszErrorMsg, (aODBCINSTErrorMsgs[*pnErrorCode]).szMsg );
        }
        return SQL_SUCCESS;
    }
}


