/**********************************************************************
 * SQLTables
 *
 **********************************************************************
 *
 * This code was created by Peter Harvey (mostly during Christmas 98/99).
 * This code is LGPL. Please ensure that this message remains in future
 * distributions and uses of this code (thats about all I get out of it).
 * - Peter Harvey pharvey@codebydesign.com
 *
 **********************************************************************/

#ifdef OSXHEADER
#include <i386/types.h>
#endif

#if defined(__bsdi__)
# include <sys/types.h>
#endif

#if HAVE_DIRENT_H
# include <dirent.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) (dirent)->d_namlen
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

#include "driver.h"

SQLRETURN SQLTables(  	SQLHSTMT    hDrvStmt,
						SQLCHAR     *szCatalogName,
						SQLSMALLINT nCatalogNameLength,
						SQLCHAR     *szSchemaName,
						SQLSMALLINT nSchemaNameLength,
						SQLCHAR     *szTableName,
						SQLSMALLINT nTableNameLength,
						SQLCHAR     *szTableType,
						SQLSMALLINT nTableTypeLength )
{
    HDRVSTMT	hStmt		= (HDRVSTMT)hDrvStmt;

    /* SANITY CHECKS */
    if( !hStmt )
        return SQL_INVALID_HANDLE;

	sprintf((char*) hStmt->szSqlMsg, "START: hStmt = %p szTableType = %s", hStmt, szTableType );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );

/********************************************************/
/* DRIVER SPECIFIC               						*/
    {
        HRESULTSET      hResultSet;
        HCOLUMN         hColumn;
        HROW            hRow;
        struct dirent **namelist;
        struct stat     statbuf;
        int             nTables;
        int             n;
        HDRVDBC         hDbc    = (HDRVDBC)hStmt->hDbc;
        char            szFileWithPath[FILENAME_MAX];
        char *          sz;
        int             alphasort();

        /* GET RID OF SOME WARNINGS */
        sz = (char*)szCatalogName;
        sz = (char*)szSchemaName;

        ResetStmt_( hStmt );

        /* CREATE A RESULTSET */
        hResultSet = hStmt->hStmtExtras->hResultSet = (HRESULTSET)calloc( 1, sizeof(RESULTSET) );

        /* ADD COLUMNS */
        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "TABLE_CAT" );
        hColumn->pszTable           = strdup( "-tables-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "TABLE_SCHEM" );
        hColumn->pszTable           = strdup( "-tables-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "TABLE_NAME" );
        hColumn->pszTable           = strdup( "-tables-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "TABLE_TYPE" );
        hColumn->pszTable           = strdup( "-tables-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hColumn = (HCOLUMN)malloc( sizeof(COLUMN) );
        hColumn->nLength            = 50;
        hColumn->nPrecision         = 0;
        hColumn->nType              = SQL_VARCHAR;
        hColumn->pszName            = strdup( "TABLE_REMARKS" );
        hColumn->pszTable           = strdup( "-tables-" );
        hResultSet->nCols++;
        hResultSet->hColumns = realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );
        hResultSet->hColumns[hResultSet->nCols-1] = hColumn;

        hResultSet->nCol            = 0; /* before first col */

        /* ADD ROWS (scan dir) */
        if ( !szTableType || !strcasecmp((char*) szTableType, "TABLE" ) )
        {
#ifdef __OS2__

	    DIR *dirp  =0;

	    dirp = opendir(hDbc->hDbcExtras->pszDirectory);
	    if(dirp)
	    	{
		struct dirent *dp	= 0;
		while ((dp = readdir (dirp)))
		{
			if (dp->d_name[0] != '.')
			{
                	    sprintf( szFileWithPath, "%s/%s", hDbc->hDbcExtras->pszDirectory, dp->d_name );
                	    if ( stat( szFileWithPath, &statbuf ) )
                	    {
                	        logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "Unable to stat file" );
                	    }
                	    if ( S_ISREG( statbuf.st_mode ) )
                	    {
                    		if ( !szTableName || !strcmp((char*) szTableName, namelist[n]->d_name ) )
                    		{
                        	hRow = (HROW)calloc( 1, sizeof(HVALUE) * hResultSet->nCols );
                        	hRow[0] = strdup( hDbc->hDbcExtras->pszDatabase );
                        	hRow[1] = strdup( hDbc->hDbcExtras->pszDatabase );
                        	hRow[2] = strdup( dp->d_name );
                        	hRow[3] = strdup( "TABLE" );
                        	hRow[4] = strdup( "" );
        
                        	hResultSet->nRows++;
                        	hResultSet->hRows = realloc( hResultSet->hRows, sizeof(HROW) * hResultSet->nRows );
                        	hResultSet->hRows[hResultSet->nRows-1] = hRow;
                		}
                	    }


			}
		}
	        closedir(dirp);
	        }

#else	
            nTables = scandir( hDbc->hDbcExtras->pszDirectory, &namelist, 0, alphasort );
            for ( n=0; n<nTables; n++ )
            {
                sprintf( szFileWithPath, "%s/%s", hDbc->hDbcExtras->pszDirectory, namelist[n]->d_name );
                if ( stat( szFileWithPath, &statbuf ) )
                {
                    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_WARNING, "Unable to stat file" );
                }
    
                if ( S_ISREG( statbuf.st_mode ) )
                {
                    if ( !szTableName || !strcmp((char*) szTableName, namelist[n]->d_name ) )
                    {
                        hRow = (HROW)calloc( 1, sizeof(HVALUE) * hResultSet->nCols );
                        hRow[0] = strdup( hDbc->hDbcExtras->pszDatabase );
                        hRow[1] = strdup( hDbc->hDbcExtras->pszDatabase );
                        hRow[2] = strdup( namelist[n]->d_name );
                        hRow[3] = strdup( "TABLE" );
                        hRow[4] = strdup( "" );
        
                        hResultSet->nRows++;
                        hResultSet->hRows = realloc( hResultSet->hRows, sizeof(HROW) * hResultSet->nRows );
                        hResultSet->hRows[hResultSet->nRows-1] = hRow;
                    }
                }
            }
#endif
    
            hResultSet->nRow            = 0; /* before first row */
    
            hStmt->nRowsAffected        = hResultSet->nRows;
        }
    }
/********************************************************/

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END: Success" );
    return SQL_SUCCESS;
}



