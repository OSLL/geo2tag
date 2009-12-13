/********************************************************************
 * SQLSpecialColumns
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

/* THIS IS CORRECT...
SQLRETURN SQLSpecialColumns(  SQLHSTMT        hDrvStmt,
									SQLSMALLINT     nColumnType,
									SQLCHAR         *szCatalogName,
									SQLSMALLINT     nCatalogNameLength,
									SQLCHAR         *szSchemaName,
									SQLSMALLINT     nSchemaNameLength,
									SQLCHAR         *szTableName,
									SQLSMALLINT     nTableNameLength,
									SQLSMALLINT     nScope,
									SQLSMALLINT     nNullable )
THIS WORKS... */
SQLRETURN SQLSpecialColumns(  SQLHSTMT        hDrvStmt,
									UWORD     nColumnType,
									UCHAR         *szCatalogName,
									SWORD     nCatalogNameLength,
									UCHAR         *szSchemaName,
									SWORD     nSchemaNameLength,
									UCHAR         *szTableName,
									SWORD     nTableNameLength,
									UWORD     nScope,
									UWORD     nNullable )
{
    HDRVSTMT hStmt	= (HDRVSTMT)hDrvStmt;

	/* SANITY CHECKS */
    if( hStmt == SQL_NULL_HSTMT )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hStmt->szSqlMsg, "hStmt = %p", hStmt );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hStmt->szSqlMsg );

	if ( !szTableName )
	{
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_ERROR, "Valid szTableName required" );
		return SQL_ERROR;
	}

    /*
	if ( nColumnType != SQL_BEST_ROWID )
	{
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_ERROR, "Column Type requested is not supported. Try SQL_BEST_ROWID." );
		return SQL_ERROR;
	}
    */

/********************************************************/
/* DRIVER SPECIFIC               						*/
    {
        HRESULTSET  hResultSet;
        HCOLUMN     hColumn;
        char        *sz;

        /* TO AVOID COMPILE WARNINGS */
        sz = (char*)szCatalogName;     
        sz = (char*)szSchemaName;

        /* RESET */
        ResetStmt_( hStmt );

        /* CREATE A RESULTSET */
        hResultSet = hStmt->hStmtExtras->hResultSet = (HRESULTSET)calloc( 1, sizeof(RESULTSET) );

        /* ADD COLUMNS */
        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "SCOPE" );
        hColumn->pszTable           = strdup( "-specialcolumns-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "COLUMN_NAME" );
        hColumn->pszTable           = strdup( "-specialcolumns-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "DATA_TYPE" );
        hColumn->pszTable           = strdup( "-specialcolumns-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "TYPE_NAME" );
        hColumn->pszTable           = strdup( "-specialcolumns-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "COLUMN_SIZE" );
        hColumn->pszTable           = strdup( "-specialcolumns-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "BUFFER_LENGTH" );
        hColumn->pszTable           = strdup( "-specialcolumns-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "DECIMAL_DIGITS" );
        hColumn->pszTable           = strdup( "-specialcolumns-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "PSEUDO_COLUMN" );
        hColumn->pszTable           = strdup( "-specialcolumns-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hStmt->nRowsAffected        = 0;
    }

/********************************************************/

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "SQL_SUCCESS" );
	return SQL_SUCCESS;
}



