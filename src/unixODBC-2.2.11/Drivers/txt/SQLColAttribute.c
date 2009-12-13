/**********************************************************************
 * SQLColAttribute
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

SQLRETURN SQLColAttribute(	SQLHSTMT        hDrvStmt,
							SQLUSMALLINT    nCol,
							SQLUSMALLINT    nFieldIdentifier,
							SQLPOINTER      pszValue,
							SQLSMALLINT     nValueLengthMax,
							SQLSMALLINT     *pnValueLength,
							SQLPOINTER      pnValue )
{
    HDRVSTMT	hStmt	= (HDRVSTMT)hDrvStmt;
    HDRVDBC     hDbc;
    int         bNumericResult  = 1;
    int         nValue          = 0;
    char        szValue[1024];
    int         ret = SQL_SUCCESS;

    /* SANITY CHECKS */
    if( !hStmt )
        return SQL_INVALID_HANDLE;
	if ( !hStmt->hStmtExtras )
		return SQL_INVALID_HANDLE;

    hDbc        = (HDRVDBC)hStmt->hDbc;
    *szValue    = '\0';

    sprintf((char*) hStmt->szSqlMsg, "START: hStmt = %p nCol = %d nAttribute = %d pszValue = %p pnValueLength = %p", hStmt, nCol, nFieldIdentifier, pszValue, pnValueLength );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );

	if ( nCol < 1 )
	{
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Column requested is less than 1." );
		return SQL_ERROR;
	}

/********************************************************/
/* DRIVER SPECIFIC               						*/
    {
        HRESULTSET  hResultSet;
        HCOLUMN     hColumn;

        hResultSet = hStmt->hStmtExtras->hResultSet;
        if ( !hResultSet )
        {
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: No result set." );
            return SQL_ERROR;
        }

        if ( nCol > hResultSet->nCols )
        {
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Column requested is greater than availible columns." );
            return SQL_ERROR;
        }

        hColumn = (hResultSet->hColumns)[nCol - 1];

        switch( nFieldIdentifier )
        {
        case SQL_DESC_AUTO_UNIQUE_VALUE:
            nValue = 0;
    		break;
        case SQL_DESC_BASE_COLUMN_NAME:
            bNumericResult = 0;
            if ( hColumn->pszName )
                strncpy( szValue, hColumn->pszName, nValueLengthMax );
            else
                strncpy( szValue, "Unknown", nValueLengthMax );
    		break;
    	case SQL_DESC_BASE_TABLE_NAME:
            bNumericResult = 0;
            if ( hColumn->pszTable )
                strncpy( szValue, hColumn->pszTable, nValueLengthMax );
            else
                strncpy( szValue, "Unknown", nValueLengthMax );
    		break;
        case SQL_DESC_CASE_SENSITIVE:
            nValue = 0;
    		break;
        case SQL_DESC_CATALOG_NAME:
            bNumericResult = 0;
            if ( hDbc->hDbcExtras->pszDatabase )
                strncpy( szValue, hDbc->hDbcExtras->pszDatabase, nValueLengthMax );
            else
                strncpy( szValue, "Unknown", nValueLengthMax );
    		break;
        case SQL_DESC_CONCISE_TYPE:
            nValue = hColumn->nType;
    		break;
        case SQL_DESC_COUNT:
            nValue = hResultSet->nCols;
    		break;
        case SQL_DESC_DISPLAY_SIZE:  /* *** */
            nValue = hColumn->nLength;
    		break;
        case SQL_DESC_FIXED_PREC_SCALE:
            nValue = 0;
    		break;
        case SQL_DESC_LABEL: /* *** */
            bNumericResult = 0;
            if ( hColumn->pszName )
                strncpy( szValue, hColumn->pszName, nValueLengthMax );
            else
                strncpy( szValue, "Unknown", nValueLengthMax );
    		break;
        case SQL_DESC_LENGTH:
            nValue = hColumn->nLength;
    		break;
    	case SQL_DESC_LITERAL_PREFIX:
            bNumericResult = 0;
            strncpy( szValue, "", nValueLengthMax );
    		break;
    	case SQL_DESC_LITERAL_SUFFIX:
            bNumericResult = 0;
            strncpy( szValue, "", nValueLengthMax );
    		break;
    	case SQL_DESC_LOCAL_TYPE_NAME:
            bNumericResult = 0;
            strncpy( szValue, "", nValueLengthMax );
    		break;
    	case SQL_DESC_NAME:
            bNumericResult = 0;
            if ( hColumn->pszName )
                strncpy( szValue, hColumn->pszName, nValueLengthMax );
            else
                strncpy( szValue, "Unknown", nValueLengthMax );
    		break;
        case SQL_DESC_NULLABLE:
            nValue = SQL_NO_NULLS;
    		break;
        case SQL_DESC_NUM_PREC_RADIX:
            nValue = 0;
    		break;
        case SQL_DESC_OCTET_LENGTH:
            nValue = hColumn->nLength;
    		break;
    	case SQL_DESC_PRECISION:
            nValue = 0;
    		break;
    	case SQL_DESC_SCALE:
            nValue = 0;
    		break;
    	case SQL_DESC_SCHEMA_NAME:
            bNumericResult = 0;
            strncpy( szValue, "", nValueLengthMax );
    		break;
        case SQL_DESC_SEARCHABLE:
            nValue = SQL_PRED_SEARCHABLE;
    		break;
    	case SQL_DESC_TABLE_NAME:
            bNumericResult = 0;
            if ( hColumn->pszTable )
                strncpy( szValue, hColumn->pszTable, nValueLengthMax );
            else
                strncpy( szValue, "Unknown", nValueLengthMax );
    		break;
        case SQL_DESC_TYPE:
            nValue = hColumn->nType;
    		break;
    	case SQL_DESC_TYPE_NAME:
            bNumericResult = 0;
            strncpy( szValue, "VARCHAR", nValueLengthMax );
    		break;
        case SQL_DESC_UNNAMED:
            nValue = SQL_NAMED;
    		break;
        case SQL_DESC_UNSIGNED:
            nValue = SQL_TRUE;
    		break;
        case SQL_DESC_UPDATABLE:
            nValue = SQL_ATTR_READWRITE_UNKNOWN;
    		break;
        default:
    			logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Unsupported attribute." );
                return SQL_ERROR;
        }
    }
/********************************************************/

    if ( bNumericResult && pnValue )
    {
        *(int*)pnValue = nValue;
        sprintf((char*) hStmt->szSqlMsg, "Value = %d", *(int*)pnValue );
        logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );
    }
    else if ( !bNumericResult && pszValue )
    {
        if ( strlen( szValue ) > nValueLengthMax )
        {
            strncpy( pszValue, szValue, nValueLengthMax );
            ret = SQL_SUCCESS_WITH_INFO;
        }
        else
            strcpy( pszValue, szValue );

        if ( pnValueLength && pszValue )
            *pnValueLength = strlen( szValue );

        sprintf((char*) hStmt->szSqlMsg, "Value = %s", szValue );
        logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );
    }
    else
    {
        logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Bad argument. Pointer for result not given." );
        return SQL_ERROR;
    }

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "END: Success." );
    return ret;
}



