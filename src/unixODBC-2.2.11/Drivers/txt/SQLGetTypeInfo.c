/**********************************************************************
 * SQLGetTypeInfo
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

HROW GetTypeInfo_( HRESULTSET hResultSet, SQLSMALLINT nType )
{
    HROW            hRow        = 0;
    char            szValue[FILENAME_MAX];

    switch ( nType )
    {
    case SQL_VARCHAR:
        hRow = (HROW)calloc( 1, sizeof(HVALUE) * hResultSet->nCols );
        hRow[0] = strdup( "VARCHAR" );
        sprintf( szValue, "%d", SQL_VARCHAR );
        hRow[1] = strdup( szValue );
        sprintf( szValue, "%d", DEFAULTCOLUMNLENGTH );
        hRow[2] = strdup( szValue );
        hRow[3] = strdup( "'" );
        hRow[4] = strdup( "'" );
        hRow[5] = strdup( "length" );
        sprintf( szValue, "%d", SQL_NO_NULLS );
        hRow[6] = strdup( szValue );
        sprintf( szValue, "%d", SQL_TRUE );
        hRow[7] = strdup( szValue );
        sprintf( szValue, "%d", SQL_PRED_BASIC );
        hRow[8] = strdup( szValue );
        hRow[9] = 0;
        sprintf( szValue, "%d", SQL_FALSE );
        hRow[10] = strdup( szValue );
        sprintf( szValue, "%d", SQL_FALSE );
        hRow[11] = strdup( szValue );
        hRow[12] = 0;
        hRow[13] = 0;
        hRow[14] = 0;
        sprintf( szValue, "%d", SQL_VARCHAR );
        hRow[15] = strdup( szValue );
        hRow[16] = 0;
        hRow[17] = 0;
        hRow[18] = 0;
        break;
    case SQL_CHAR:
    case SQL_LONGVARCHAR:
/*    case SQL_WCHAR:           */
/*    case SQL_WVARCHAR:        */
/*    case SQL_WLONGVARCHAR:    */
    case SQL_DECIMAL:
    case SQL_NUMERIC:
    case SQL_SMALLINT:
    case SQL_INTEGER:
    case SQL_REAL:
    case SQL_FLOAT:
    case SQL_DOUBLE:
    case SQL_BIT:
    case SQL_TINYINT:
    case SQL_BIGINT:
    case SQL_BINARY:
    case SQL_VARBINARY:
    case SQL_LONGVARBINARY:
    case SQL_TYPE_DATE:
    case SQL_TYPE_TIME:
    case SQL_TYPE_TIMESTAMP:
    case SQL_INTERVAL_MONTH:
    case SQL_INTERVAL_YEAR:
    case SQL_INTERVAL_YEAR_TO_MONTH:
    case SQL_INTERVAL_DAY:
    case SQL_INTERVAL_HOUR:
    case SQL_INTERVAL_MINUTE:
    case SQL_INTERVAL_SECOND:
    case SQL_INTERVAL_DAY_TO_HOUR:
    case SQL_INTERVAL_DAY_TO_MINUTE:
    case SQL_INTERVAL_DAY_TO_SECOND:
    case SQL_INTERVAL_HOUR_TO_MINUTE:
    case SQL_INTERVAL_HOUR_TO_SECOND:
    case SQL_INTERVAL_MINUTE_TO_SECOND:
    case SQL_GUID:
    default:
        return 0;
    }

    hResultSet->nRows++;
    hResultSet->hRows = realloc( hResultSet->hRows, sizeof(HROW) * hResultSet->nRows );
    hResultSet->hRows[hResultSet->nRows-1] = hRow;

    return hRow;
}

SQLRETURN SQLGetTypeInfo( SQLHSTMT      hDrvStmt,
                          SQLSMALLINT   nType )
{
    HDRVSTMT	hStmt		= (HDRVSTMT)hDrvStmt;

    /* SANITY CHECKS */
    if( !hStmt )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hStmt->szSqlMsg, "START: hStmt = %p nType = %d", hStmt, nType );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );

/********************************************************/
/* DRIVER SPECIFIC               						*/
    {
        HDRVDBC         hDbc        = (HDRVDBC)hStmt->hDbc;
        HRESULTSET      hResultSet  = 0;

        /* RESET */
        ResetStmt_( hStmt );

        /* CREATE A RESULTSET */
        hResultSet = hStmt->hStmtExtras->hResultSet = (HRESULTSET)calloc( 1, sizeof(RESULTSET) );

        /* ADD COLUMNS */
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "TYPE_NAME", SQL_VARCHAR, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "DATA_TYPE", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "COLUMN_SIZE", SQL_INTEGER, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "LITERAL_PREFIX", SQL_VARCHAR, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "LITERAL_SUFFIX", SQL_VARCHAR, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "CREATE_PARAMS", SQL_VARCHAR, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "NULLABLE", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "CASE_SENSITIVE", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "SEARCHABLE", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "UNSIGNED_ATTRIBUTE", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "FIXED_PREC_SCALE", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "AUTO_UNIQUE_VALUE", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "LOCAL_TYPE_NAME", SQL_VARCHAR, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "MINIMUM_SCALE", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "MAXIMUM_SCALE", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "SQL_DATA_TYPE", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "SQL_DATETIME_SUB", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "NUM_PREC_RADIX", SQL_INTEGER, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-types-", "INTERVAL_PRECISION", SQL_SMALLINT, 0, 0 );

        /* PROCESS TYPES */
        /* "ordered by DATA_TYPE and then by how closely the data type maps to the corresponding ODBC SQL data type" */
        if ( nType == SQL_ALL_TYPES )
        {
            GetTypeInfo_( hResultSet,  SQL_CHAR );
            GetTypeInfo_( hResultSet,  SQL_VARCHAR );
            GetTypeInfo_( hResultSet,  SQL_LONGVARCHAR );
/*
            GetTypeInfo_( hResultSet,  SQL_WCHAR );
            GetTypeInfo_( hResultSet,  SQL_WVARCHAR );
            GetTypeInfo_( hResultSet,  SQL_WLONGVARCHAR );
*/            
            GetTypeInfo_( hResultSet,  SQL_DECIMAL );
            GetTypeInfo_( hResultSet,  SQL_NUMERIC );
            GetTypeInfo_( hResultSet,  SQL_SMALLINT );
            GetTypeInfo_( hResultSet,  SQL_INTEGER );
            GetTypeInfo_( hResultSet,  SQL_REAL );
            GetTypeInfo_( hResultSet,  SQL_FLOAT );
            GetTypeInfo_( hResultSet,  SQL_DOUBLE );
            GetTypeInfo_( hResultSet,  SQL_BIT );
            GetTypeInfo_( hResultSet,  SQL_TINYINT );
            GetTypeInfo_( hResultSet,  SQL_BIGINT );
            GetTypeInfo_( hResultSet,  SQL_BINARY );
            GetTypeInfo_( hResultSet,  SQL_VARBINARY );
            GetTypeInfo_( hResultSet,  SQL_LONGVARBINARY );
            GetTypeInfo_( hResultSet,  SQL_TYPE_DATE );
            GetTypeInfo_( hResultSet,  SQL_TYPE_TIME );
            GetTypeInfo_( hResultSet,  SQL_TYPE_TIMESTAMP );
            GetTypeInfo_( hResultSet,  SQL_INTERVAL_MONTH );
            GetTypeInfo_( hResultSet,  SQL_INTERVAL_YEAR );
            GetTypeInfo_( hResultSet,  SQL_INTERVAL_YEAR_TO_MONTH );
            GetTypeInfo_( hResultSet,  SQL_INTERVAL_DAY );
            GetTypeInfo_( hResultSet,  SQL_INTERVAL_HOUR );
            GetTypeInfo_( hResultSet,  SQL_INTERVAL_MINUTE );
            GetTypeInfo_( hResultSet,  SQL_INTERVAL_SECOND );
            GetTypeInfo_( hResultSet,  SQL_INTERVAL_DAY_TO_HOUR );
            GetTypeInfo_( hResultSet,  SQL_INTERVAL_DAY_TO_MINUTE );
            GetTypeInfo_( hResultSet,  SQL_INTERVAL_DAY_TO_SECOND );
            GetTypeInfo_( hResultSet,  SQL_INTERVAL_HOUR_TO_MINUTE );
            GetTypeInfo_( hResultSet,  SQL_INTERVAL_HOUR_TO_SECOND );
            GetTypeInfo_( hResultSet,  SQL_INTERVAL_MINUTE_TO_SECOND );
            GetTypeInfo_( hResultSet,  SQL_GUID );
        }
        else
            GetTypeInfo_( hResultSet, nType );

        hResultSet->nCol            = 0; /* before first col */
        hResultSet->nRow            = 0; /* before first row */

        hStmt->nRowsAffected        = hResultSet->nRows;
    }
/********************************************************/

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success." );
    return SQL_SUCCESS;
}


