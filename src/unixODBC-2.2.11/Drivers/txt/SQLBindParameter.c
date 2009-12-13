/**********************************************************************
 * SQLBindParameter
 *
 **********************************************************************
 *
 * This code was created by Peter Harvey (mostly during Christmas 98/99).
 * This code is LGPL. Please ensure that this message remains in future
 * distributions and uses of this code (thats about all I get out of it).
 * - Peter Harvey pharvey@codebydesign.com
 *
 **********************************************************************/

#include "driver.h"

SQLRETURN  SQLBindParameter( SQLHSTMT           hDrvStmt,
							 SQLUSMALLINT       nParameterNumber,
							 SQLSMALLINT		nIOType,
							 SQLSMALLINT        nBufferType,
							 SQLSMALLINT        nParamType,
							 SQLULEN            nParamLength,
							 SQLSMALLINT        nScale,
							 SQLPOINTER         pData,
							 SQLLEN             nBufferLength,
							 SQLLEN             *pnLengthOrIndicator
						   )
{
    HDRVSTMT hStmt	= (HDRVSTMT)hDrvStmt;

    /* SANITY CHECKS */
    if( NULL == hStmt )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hStmt->szSqlMsg, "hStmt=%p nParameterNumber=%d nIOType=%d nBufferType=%d nParamType=%d nParamLength=%d nScale=%d pData=%p nBufferLength=%d *pnLengthOrIndicator=%p",hStmt,nParameterNumber,nIOType,nBufferType,nParamType,nParamLength,nScale,pData,nBufferLength, *pnLengthOrIndicator );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hStmt->szSqlMsg );

	if ( nParameterNumber < 1 )
	{
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "Desired parameter is less than 1." );
		return SQL_ERROR;
	}

/********************************************************/
/* DRIVER SPECIFIC               						*/
  {
    HSQPPARAM  pParam;

	if ( nIOType != SQL_PARAM_INPUT )
	{
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR This IOType not currently supported" );
		return SQL_ERROR;
	}
	if ( nBufferType != SQL_C_CHAR )
	{
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR This BufferType not currently supported" );
		return SQL_ERROR;
	}

	if ( !hStmt->hStmtExtras->hParams )
	{
		hStmt->hStmtExtras->hParams = lstOpen();
	}

    pParam                     = (HSQPPARAM)malloc( sizeof(SQPPARAM) );
    if ( *pnLengthOrIndicator == SQL_NULL_DATA )
	    pParam->pszValue       = (char *)strdup( "" );
    else
    {
        pParam->pszValue       = (char *)calloc(1, *pnLengthOrIndicator+1);
        memcpy(pParam->pszValue, pData, *pnLengthOrIndicator);
    }

    lstAppend( hStmt->hStmtExtras->hParams, pParam );

  }
/********************************************************/

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "SQL_SUCCESS" );

    return SQL_SUCCESS;
}


