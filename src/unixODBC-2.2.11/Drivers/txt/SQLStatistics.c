/********************************************************************
 * SQLStatistics
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


SQLRETURN SQLStatistics(	SQLHSTMT        hDrvStmt,
							SQLCHAR         *szCatalogName,
							SQLSMALLINT     nCatalogNameLength,
							SQLCHAR         *szSchemaName,
							SQLSMALLINT     nSchemaNameLength,
							SQLCHAR         *szTableName,      		/* MUST BE SUPPLIED */
							SQLSMALLINT     nTableNameLength,
							SQLUSMALLINT    nTypeOfIndex,
							SQLUSMALLINT    nReserved    )
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

/********************************************************/
/* DRIVER SPECIFIC               						*/
    {
        HRESULTSET  hResultSet;
        HCOLUMN     hColumn;

        /* RESET */
        ResetStmt_( hStmt );

        /* CREATE A RESULTSET */
        hResultSet = hStmt->hStmtExtras->hResultSet = (HRESULTSET)calloc( 1, sizeof(RESULTSET) );

        /* ADD COLUMNS */
        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "TABLE_CAT" );
        hColumn->pszTable           = strdup( "-statistics-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "TABLE_SCHEM" );
        hColumn->pszTable           = strdup( "-statistics-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "TABLE_NAME" );
        hColumn->pszTable           = strdup( "-statistics-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "NON_UNIQUE" );
        hColumn->pszTable           = strdup( "-statistics-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "INDEX_QUALIFIER" );
        hColumn->pszTable           = strdup( "-statistics-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "INDEX_NAME" );
        hColumn->pszTable           = strdup( "-statistics-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "TYPE" );
        hColumn->pszTable           = strdup( "-statistics-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "ORDINAL_POSITION" );
        hColumn->pszTable           = strdup( "-statistics-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "COLUMN_NAME" );
        hColumn->pszTable           = strdup( "-statistics-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "ASC_OR_DESC" );
        hColumn->pszTable           = strdup( "-statistics-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "CARDINALITY" );
        hColumn->pszTable           = strdup( "-statistics-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "PAGES" );
        hColumn->pszTable           = strdup( "-statistics-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "FILTER_CONDITION" );
        hColumn->pszTable           = strdup( "-statistics-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;


        hStmt->nRowsAffected        = 0;
    }
/********************************************************/

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "SQL_SUCCESS" );
	return SQL_SUCCESS;
}


