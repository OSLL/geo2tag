/********************************************************************
 * SQLColumns
 *
 **********************************************************************
 *
 * This code was created by Peter Harvey (mostly during Christmas 98/99).
 * This code is LGPL. Please ensure that this message remains in future
 * distributions and uses of this code (thats about all I get out of it).
 * - Peter Harvey pharvey@codebydesign.com
 *
 ********************************************************************/

#include "driver.h"

SQLRETURN SQLColumns( 	SQLHSTMT    hDrvStmt,
						SQLCHAR     *szCatalogName,
						SQLSMALLINT nCatalogNameLength,
						SQLCHAR     *szSchemaName,
						SQLSMALLINT nSchemaNameLength,
						SQLCHAR     *szTableName,
						SQLSMALLINT nTableNameLength,
						SQLCHAR     *szColumnName,
						SQLSMALLINT nColumnNameLength )
{
    HDRVSTMT	hStmt		= (HDRVSTMT)hDrvStmt;

    /* SANITY CHECKS */
    if( !hStmt )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hStmt->szSqlMsg, "hStmt = %p", hStmt );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );

	if ( szTableName == NULL || szTableName[0] == '\0' )
	{
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "SQL_ERROR Must supply a valid table name" );
		return SQL_ERROR;
	}

/********************************************************/
/* DRIVER SPECIFIC               						*/

    /* AVOID COMPILE WARNINGS */
    {
        char * sz;
        sz = (char*)szCatalogName;
        sz = (char*)szSchemaName;
    }

    {
        HDRVDBC         hDbc        = (HDRVDBC)hStmt->hDbc;
        HRESULTSET      hResultSet  = 0;
        HROW            hRow        = 0;
        HTABLE          hTable      = 0;
        HCOLUMNS        hColumns    = 0;
        long            nCols       = 0;
        long            nCol        = 0;
        char            szValue[FILENAME_MAX];

        /* RESET */
        ResetStmt_( hStmt );

        /* GET TABLE HEADER */
        if ( !IOTableOpen( &hTable, hStmt, (char*)szTableName, IOSelect ) )
        {
            sprintf((char*) hStmt->szSqlMsg, "Could not open %s.", szTableName );
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, (char*)hStmt->szSqlMsg );
            return SQL_ERROR;
        }

        if ( !IOTableHeaderRead( hTable, &hColumns, &nCols ) )
        {
            sprintf((char*) hStmt->szSqlMsg, "Could not read table header for %s.", szTableName );
            logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hStmt->szSqlMsg );
            IOTableClose( &hTable );
            return SQL_ERROR;
        }

        IOTableClose( &hTable );

        /* CREATE A RESULTSET */
        hResultSet = hStmt->hStmtExtras->hResultSet = (HRESULTSET)calloc( 1, sizeof(RESULTSET) );

        /* ADD COLUMNS */
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "TABLE_CAT", SQL_VARCHAR, 50, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "TABLE_SCHEM", SQL_VARCHAR, 50, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "TABLE_NAME", SQL_VARCHAR, 50, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "COLUMN_NAME", SQL_VARCHAR, 50, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "DATA_TYPE", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "TYPE_NAME", SQL_VARCHAR, 50, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "COLUMN_SIZE", SQL_INTEGER, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "BUFFER_LENGTH", SQL_INTEGER, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "DECIMAL_DIGITS", SQL_SMALLINT, 0, 0 );
        
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "NUM_PREC_RADIX", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "NULLABLE", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "REMARKS", SQL_VARCHAR, 50, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "COLUMN_DEF", SQL_VARCHAR, 50, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "SQL_DATA_TYPE", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "SQL_DATETIME_SUB", SQL_SMALLINT, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "CHAR_OCTET_LENGTH", SQL_INTEGER, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "ORDINAL_POSITION", SQL_INTEGER, 0, 0 );

        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = CreateColumn_( "-columns-", "IS_NULLABLE", SQL_VARCHAR, 50, 0 );

        /* PROCESS COLUMNS */
        for ( nCol = 0; nCol < nCols; nCol++ )
        {
            if ( !szColumnName || (!strcasecmp((char*) szColumnName, hColumns[nCol]->pszName )) )
            {
                /* ADD ROW */
                hRow = (HROW)calloc( 1, sizeof(HVALUE) * hResultSet->nCols );
                hRow[0] = strdup( hDbc->hDbcExtras->pszDatabase );
                hRow[1] = strdup( hDbc->hDbcExtras->pszDatabase );
                hRow[2] = strdup((char*) szTableName );
                hRow[3] = strdup( hColumns[nCol]->pszName );
                sprintf( szValue, "%d", SQL_VARCHAR );
                hRow[4] = strdup( szValue );
                hRow[5] = strdup( "VARCHAR" );
                sprintf( szValue, "%d", hColumns[nCol]->nLength );
                hRow[6] = strdup( szValue );
                hRow[7] = strdup( hRow[6] );
                hRow[8] = 0;
                hRow[9] = 0;
                sprintf( szValue, "%d", SQL_NO_NULLS );
                hRow[10] = strdup( szValue );
                hRow[11] = strdup( "" );
                hRow[12] = strdup( "''" );
                hRow[13] = strdup( hRow[4] );
                hRow[14] = 0;
                hRow[15] = strdup( hRow[6] );
                sprintf( szValue, "%d", nCol + 1 );
                hRow[16] = strdup( szValue );
                hRow[17] = strdup( "NO" );
    
                hResultSet->nRows++;
                hResultSet->hRows = realloc( hResultSet->hRows, sizeof(HROW) * hResultSet->nRows );
                hResultSet->hRows[hResultSet->nRows-1] = hRow;
            }
        }

        FreeColumns_( &hColumns, nCols );

        hResultSet->nCol            = 0; /* before first col */
        hResultSet->nRow            = 0; /* before first row */

        hStmt->nRowsAffected        = hResultSet->nRows;
    }
/********************************************************/

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "SQL_SUCCESS" );
    return SQL_SUCCESS;
}



