/**********************************************************************
 * SQLGetData
 *
 * 1. MiniSQL server sends all data as ascii strings so things are
 * simplified. We always convert from string to nTargetType.
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

SQLRETURN SQLGetData_(		SQLHSTMT      hDrvStmt,
    						SQLUSMALLINT  nCol,						/* 1 based */
    						SQLSMALLINT   nTargetType,				/* C DATA TYPE */
    						SQLPOINTER    pTarget,
    						SQLLEN        nTargetLength,
    						SQLLEN        *pnLengthOrIndicator )
{
    HDRVSTMT	hStmt			= (HDRVSTMT)hDrvStmt;

	/* SANITY CHECKS */
	if ( hStmt == SQL_NULL_HSTMT )
		return SQL_INVALID_HANDLE;

	if ( hStmt->hStmtExtras == SQL_NULL_HSTMT )
		return SQL_INVALID_HANDLE;

    sprintf((char*) hStmt->szSqlMsg, "START: hStmt = %p nCol = %d nTargetType = %d", hStmt, nCol, nTargetType );
	logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, (char*)hStmt->szSqlMsg );

    if ( !pTarget )
    {
        logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: pTarget is NULL." );
        return SQL_ERROR;
    }

    if ( nCol < 1 )
    {
        logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Column number less than 1." );
        return SQL_ERROR;
    }

/********************************************************/
/* DRIVER SPECIFIC               						*/
    {
        HRESULTSET  hResultSet;
        HROW        hRow;
        HVALUE      hValue;

        hResultSet = hStmt->hStmtExtras->hResultSet;
        if ( !hResultSet )
        {
			logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: No ResultSet." );
            return SQL_ERROR;
        }

        if ( nCol > hResultSet->nCols )
        {
			logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Column number greater than availible columns." );
            return SQL_ERROR;
        }

        if ( (hResultSet->nRow < 1) || (hResultSet->nRow > hResultSet->nRows) )
        {
			logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Invalid row." );
            return SQL_ERROR;
        }

        hRow    = hResultSet->hRows[hResultSet->nRow - 1];
        hValue  = hRow[nCol - 1];

        if ( !hValue )
        {
			if ( pnLengthOrIndicator )
            {
                *pnLengthOrIndicator = SQL_NULL_DATA;
            }
            else
            {
                logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Column NULL and indicator NOT given." );
                return SQL_ERROR;
            }
        }
        else
        {
            switch ( nTargetType )
            {
            case SQL_C_LONG:
                *((int *)pTarget) = atoi( hValue );
                if ( pnLengthOrIndicator )
                    *pnLengthOrIndicator = sizeof( int );
                break;

            case SQL_C_SHORT:
                *((short *)pTarget) = atoi( hValue );
                if ( pnLengthOrIndicator )
                    *pnLengthOrIndicator = sizeof( short );
                break;

            case SQL_C_FLOAT:
                sscanf( hValue, "%g", pTarget );
                if ( pnLengthOrIndicator )
                    *pnLengthOrIndicator = sizeof( float );
                break;

            case SQL_C_CHAR:
                strncpy( pTarget, hValue, nTargetLength );
                if ( pnLengthOrIndicator )
                    *pnLengthOrIndicator = strlen( pTarget );
                break;

            default:
                logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: TargetType not supported." );
                return SQL_ERROR;
            }
        }
	}
/********************************************************/

	logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success." );
	return SQL_SUCCESS;
}


SQLRETURN SQLGetData( SQLHSTMT      hDrvStmt,
					  SQLUSMALLINT  nCol,						/* 1 based */
					  SQLSMALLINT   nTargetType,				/* C DATA TYPE */
					  SQLPOINTER    pTarget,
					  SQLLEN    nTargetLength,
					  SQLLEN    *pnLengthOrIndicator )
{
	return SQLGetData_(	hDrvStmt,
						nCol, /* 1 based */
						nTargetType,		
						pTarget,
						nTargetLength,
						pnLengthOrIndicator );
}


