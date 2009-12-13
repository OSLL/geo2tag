#include "driver.h"


/*
 * IO ENTRY FUNCS
 *
 * - assume that hParsedSQL has a, valid, parsed SQL statement
 * - assume that hResultSet is empty
 *
 * If you want to plugin an alternate IO which accesses different data
 * then you would start by just replacing IOText.c with IO???.c
 * If you find that you need more flexability then you can replace IO.c
 *
 */
SQLRETURN IOUpdateTable( HDRVSTMT hStmt )
{
    char *          FUNC_           = "IOUpdateTable";
    HSQPUPDATE      hUpdate         = hStmt->hStmtExtras->hParsedSQL->h.hUpdate;
    HTABLE          hTable          = 0;
    HCOLUMNS        hColumns        = 0;
    long            nCols           = 0;
    long            nCol            = 0;
    HROWS           hRows           = 0;
    HROW            hRow            = 0;
    long            nRows           = 0;
    long            nRow            = 0;
    HSQPCOMPARISON  hSQPComparison  = 0;
    HSQPASSIGNMENT  hSQPAssignment  = 0;

    logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "START" );
    logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, hUpdate->pszTable );

    if ( !IOTableOpen( &hTable, hStmt, hUpdate->pszTable, IOUpdate ) )
    {
		logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_WARNING, LOG_ERROR, "Could not open table." );
		return SQL_ERROR;
    }

    /*
     * STEP 1
     *
     * Get all column headers.
     *
     */
    if ( !IOTableHeaderRead( hTable, &hColumns, &nCols ) )
    {
        IOTableClose( &hTable );
		logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_WARNING, LOG_ERROR, "Could not read table info." );
		return SQL_ERROR;
    }   

    /*
     * STEP 2
     *
     * Get all rows; updating as we go along.
     *
     */

    /* xref columns in WHERE */
    logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "xref-ing SELECT columns into interim columns." );
    IOXrefWhere( hUpdate->hWhere, hColumns, nCols );

    /* xref columns in SET */
    logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "xref-ing SET columns into interim columns." );
    lstFirst( hUpdate->hAssignments );
    while ( !lstEOL( hUpdate->hAssignments ) )
    {
        hSQPAssignment              = (HSQPASSIGNMENT)lstGet( hUpdate->hAssignments );
        hSQPAssignment->nColumn     = -1;
        for ( nCol=0; nCol < nCols; nCol++ )
        {
            if ( !strcasecmp( hSQPAssignment->pszColumn, hColumns[nCol]->pszName ) )
            {
                hSQPAssignment->nColumn = nCol;
                break;
            }
        }
        lstNext( hUpdate->hAssignments );
    }

    /* process rows */
    logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "Reading desired rows into interim data set." );
    hStmt->nRowsAffected    = 0;
    while ( IOTableRead( hTable, &hRow, nCols ) )
    {
        if ( IOWhere( hRow, hUpdate->hWhere, ((HDRVDBC)(hStmt->hDbc))->hDbcExtras ) )
        {
            /* update this row */
            hStmt->nRowsAffected++;
            lstFirst( hUpdate->hAssignments );
            while ( !lstEOL( hUpdate->hAssignments ) )
            {
                hSQPAssignment = (HSQPASSIGNMENT)lstGet( hUpdate->hAssignments );
                if ( hSQPAssignment->nColumn >= 0 )
                {
                    free( hRow[hSQPAssignment->nColumn] );
                    hRow[hSQPAssignment->nColumn] = (HVALUE)strdup( hSQPAssignment->pszValue );
                }
                lstNext( hUpdate->hAssignments );
            }
        }
        nRows++;
        hRows               = (HROWS)realloc( hRows, sizeof(HROW) * nRows );
        hRows[nRows - 1]    = hRow;
    }
    sprintf((char*) hStmt->szSqlMsg, "Updated %ld of %ld rows.", hStmt->nRowsAffected, nRows );
    logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );

    /*
     * STEP 3
     *
     * Replace file with what we have loaded.
     *
     */
    if ( !IOTableHeaderWrite( hTable, hColumns, nCols ) )
    {
        IOTableClose( &hTable );
		logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_WARNING, LOG_ERROR, "Could not write table info." );
        FreeColumns_( &hColumns, nCols );
        FreeRows_( &hRows, nRows, nCols );    
		return SQL_ERROR;
    }   
    for ( nRow=0; nRow < nRows; nRow++ )
    {
        IOTableWrite( hTable, hRows[nRow], nCols );
    }

    /*
     * STEP 5
     *
     * Finalize
     *
     */
    IOTableClose( &hTable );
    FreeColumns_( &hColumns, nCols );
    FreeRows_( &hRows, nRows, nCols );    

    logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "END" );

    return SQL_SUCCESS;
}

SQLRETURN IOSelectTable( HDRVSTMT hStmt )
{
    char *      FUNC_           = "IOSelectTable";
    HSQPSELECT  hSelect         = hStmt->hStmtExtras->hParsedSQL->h.hSelect;
    HTABLE      hTable          = 0;

    HCOLUMNS    hColumns        = 0;
    HCOLUMN     hColumn         = 0;
    long        nCols           = 0;
    long        nCol            = 0;
    HROWS       hRows           = 0;
    HROW        hRow            = 0;
    long        nRows           = 0;

    HSQPCOLUMN      hSQPColumn;
    HSQPCOMPARISON  hSQPComparison;

    HRESULTSET      hResultSet  = 0;

    sprintf((char*) hStmt->szSqlMsg, "START: Table = %s", hSelect->pszTable );
    logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );


    if ( !IOTableOpen( &hTable, hStmt, hSelect->pszTable, IOSelect ) )
    {
		logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_WARNING, LOG_WARNING, "END: Could not open table." );
		return SQL_ERROR;
    }

    /*
     * STEP 1
     *
     * Get all column headers.
     *
     */
    if ( !IOTableHeaderRead( hTable, &hColumns, &nCols ) )
    {
        IOTableClose( &hTable );
		logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_WARNING, LOG_WARNING, "END: Could not read table info." );
		return SQL_ERROR;
    }   

    /*
     * STEP 2
     *
     * Get desired rows.
     *
     * - we assume we have an AND between each
     *   comparison in our WHERE                 
     *
     */

    /* xref columns in WHERE */
    logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "xref-ing SELECT columns into interim column headers." );
    IOXrefWhere( hSelect->hWhere, hColumns, nCols );

    /* gather rows */
    logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "Reading desired rows into interim data set." );
    while ( IOTableRead( hTable, &hRow, nCols ) )
    {
        if ( IOWhere( hRow, hSelect->hWhere, ((HDRVDBC)(hStmt->hDbc))->hDbcExtras ) )
        {
            nRows++;
            hRows               = (HROWS)realloc( hRows, sizeof(HROW) * nRows );
            hRows[nRows - 1]    = hRow;
        }
        else
            FreeRow_( &hRow, nCols );
    }
    IOTableClose( &hTable );
    sprintf((char*) hStmt->szSqlMsg, "Found %ld rows.", nRows );
    logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );

    /*
     * STEP 3
     *
     * Use whatever columns we need to apply any sort we may have.
     *
     * - sort if ORDER BY
     * - use qsort() on hRows
     *
     */
    if ( hSelect->hOrderBy )
    {
        /* xref columns in ORDER BY */
        logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "xref-ing ORDER BY columns into interim data." );
        lstFirst( hSelect->hOrderBy );
        while ( !lstEOL( hSelect->hOrderBy ) )
        {
            hSQPColumn              = (HSQPCOLUMN)lstGet( hSelect->hOrderBy );
            hSQPColumn->nColumn     = -1;
            for ( nCol=0; nCol < nCols; nCol++ )
            {
                if ( !strcasecmp( hSQPColumn->pszColumn, hColumns[nCol]->pszName ) )
                {
                    hSQPColumn->nColumn = nCol;
                    break;
                }
            }
            lstNext( hSelect->hOrderBy );
        }

        logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "Sorting rows in interim data set." );
        IOSort( hSelect->hOrderBy, hSelect->nOrderDirection, hRows, 0, nRows -1 );
    }


    /*
     * STEP 4
     *
     * Reduce columns to match SELECT
     *
     * - place final result in hResultSet
     * - We could have done this as we collected rows but
     *   we needed all columns for the subsequent sort (if any).
     *   So this could be optimized based on whether or not we
     *   even have to do a sort.
     *
     */
    hResultSet          = (HRESULTSET)calloc( 1, sizeof(RESULTSET) );

    /* SELECT * was used... results much faster */
    lstFirst( hSelect->hColumns );
    hSQPColumn = (HSQPCOLUMN)lstGet( hSelect->hColumns );
    if ( (hSQPColumn->pszColumn)[0] == '*' )
    {
        logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "Adopting all columns in interim data set." );
        /* adopt all column headers         */
        hResultSet->hColumns    = hColumns;
        hResultSet->nCols       = nCols;
        hResultSet->nCol        = 0;
        /* adopt all rows and all values    */
        hResultSet->hRows       = hRows;
        hResultSet->nRows       = nRows;
        hResultSet->nRow        = 0;
    }
    /* SELECT col1, col2, col3 was used... results much slower */
    else
    {
        /* xref columns in SELECT */
        logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "xref-ing SELECT columns into interim data set." );
        lstFirst( hSelect->hColumns );
        while ( !lstEOL( hSelect->hColumns ) )
        {
            hSQPColumn          = (HSQPCOLUMN)lstGet( hSelect->hColumns );
            hSQPColumn->nColumn = -1;    
            /* column number */
            if ( isdigit( *(hSQPColumn->pszColumn) ) )
            {
                hSQPColumn->nColumn = atol( hSQPColumn->pszColumn ) - 1;
                if ( hSQPColumn->nColumn < 0 || hSQPColumn->nColumn >= nCols )
                {
                    logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "Requested column out of range." );
                    hSQPColumn->nColumn = -1;    
                }
            }
            /* column name */
            else
            {
                for ( nCol=0; nCol < nCols; nCol++ )
                {
                    if ( !strcasecmp( hSQPColumn->pszColumn , hColumns[nCol]->pszName ) )
                    {
                        hSQPColumn->nColumn = nCol;
                        break;
                    }
                }
            }
            if ( hSQPColumn->nColumn < 0 )
            {
                sprintf((char*) hStmt->szSqlMsg, "%s appears to be an invalid column name. It will be ignored.", hSQPColumn->pszColumn );
                logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );
            }
            lstNext( hSelect->hColumns );
        }

        /* copy only SELECTed column headers */
        logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "Reducing column headers." );
        lstFirst( hSelect->hColumns );
        while ( !lstEOL( hSelect->hColumns ) )
        {
            hSQPColumn = (HSQPCOLUMN)lstGet( hSelect->hColumns );
            if ( hSQPColumn->nColumn >= 0 )
            {
                hResultSet->nCols++;
                hResultSet->hColumns = (HCOLUMNS)realloc( hResultSet->hColumns, sizeof(HCOLUMN) * hResultSet->nCols );

                hColumn = hColumns[hSQPColumn->nColumn];
                hResultSet->hColumns[hResultSet->nCol] = CreateColumn_( hColumn->pszTable, hColumn->pszName, hColumn->nType, hColumn->nLength, hColumn->nPrecision );

                hResultSet->nCol++;
            }
            else
            {
                sprintf((char*) hStmt->szSqlMsg, "Could not find column header %s.", hSQPColumn->pszColumn );
                logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );
            }
            lstNext( hSelect->hColumns );
        }
        hResultSet->nCol = 0;

        /* transfer rows */
        logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "Reducing columns in data rows." );
        hResultSet->nRows   = nRows;
        hResultSet->hRows   = (HROWS)calloc( 1, sizeof(HROW) * nRows );
        for ( hResultSet->nRow=0; hResultSet->nRow < hResultSet->nRows; hResultSet->nRow++ )
        {
            /* copy only SELECTed values */
            hRow                = (HROW)calloc( 1, sizeof(HVALUE) * hResultSet->nCols );
            hResultSet->nCol    = 0;
            lstFirst( hSelect->hColumns );
            while ( !lstEOL( hSelect->hColumns ) )
            {
                hSQPColumn = (HSQPCOLUMN)lstGet( hSelect->hColumns );
                if ( hSQPColumn->nColumn >= 0 )
                {
                    hRow[hResultSet->nCol] = (HVALUE)strdup( hRows[hResultSet->nRow][hSQPColumn->nColumn] );
                    hResultSet->nCol++;
                }
                lstNext( hSelect->hColumns );
            }
            hResultSet->hRows[hResultSet->nRow] = hRow;
        }
        hResultSet->nRow = 0;

        FreeColumns_( &hColumns, hResultSet->nCols );
        FreeRows_( &hRows, hResultSet->nRows, hResultSet->nCols );
    }

    /*
     * STEP 5
     *
     * Finalize
     *
     */
    hStmt->hStmtExtras->hResultSet  = hResultSet;
    hStmt->nRowsAffected            = hResultSet->nRows;

    logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "END: Success." );

    return SQL_SUCCESS;
}

SQLRETURN IOInsertTable( HDRVSTMT hStmt )
{
    char *      FUNC_           = "IOInsertTable";
    HTABLE      hTable  = 0;
    HSQPINSERT  hInsert = 0;
    HROW        hRow    = 0;
    long        nCol    = 0;
    HCOLUMNS    hColumns        = 0;
    long        nCols           = 0;
    HSQPCOLUMN      hSQPColumn;

    hInsert = hStmt->hStmtExtras->hParsedSQL->h.hInsert;
    if ( !IOTableOpen( &hTable, hStmt, hInsert->pszTable, IOInsert ) )
    {
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_ERROR, "Could not open table." );
		return SQL_ERROR;
    }

    /*
     * STEP 1
     *
     * Get all column headers.
     *
     */
    if ( !IOTableHeaderRead( hTable, &hColumns, &nCols ) )
    {
        IOTableClose( &hTable );
		logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_WARNING, LOG_WARNING, "END: Could not read table info." );
		return SQL_ERROR;
    }   

    /*
     * STEP 2
     *
     * Make list of all column headers if not set in request.
     *
     */
    if ( !hInsert->hColumns )
    {
        for ( nCol=0; nCol < nCols; nCol++ )
        {
            sqpStoreColumn( &hInsert->hColumns, hColumns[nCol]->pszName, 0 );
        }
    }

    /*
     * STEP 3
     *
     * Search wanted columns among values.
     *
     */
    hRow = (HROW)calloc( 1, sizeof(HVALUE) * nCols );
    for ( nCol=0; nCol < nCols; nCol++ )
    {
        lstFirst( hInsert->hColumns );
        lstFirst( hInsert->hValues );
        while ( !lstEOL( hInsert->hColumns ) && !lstEOL( hInsert->hValues ) )
        {
            hSQPColumn          = (HSQPCOLUMN)lstGet( hInsert->hColumns );
            hSQPColumn->nColumn = -1;    
            /* column number */
            if ( isdigit( *(hSQPColumn->pszColumn) ) )
            {
                hSQPColumn->nColumn = atol( hSQPColumn->pszColumn ) - 1;
                if ( hSQPColumn->nColumn == nCol )
                {
                    hRow[nCol] = (HVALUE)lstGet( hInsert->hValues );
                }
            }
            /* column name */
            else
            {
                if ( !strcasecmp( hSQPColumn->pszColumn , hColumns[nCol]->pszName ) )
                {
                    hRow[nCol] = (HVALUE)lstGet( hInsert->hValues );
                }
            }
            lstNext( hInsert->hColumns );
            lstNext( hInsert->hValues );
        }
        if ( !hRow[nCol] )
            hRow[nCol] = (HVALUE)"";
    }

    IOTableWrite( hTable, hRow, nCol );

    IOTableClose( &hTable );

    hStmt->nRowsAffected = 1;

    free( hRow );

    return SQL_SUCCESS;
}

SQLRETURN IODeleteTable( HDRVSTMT hStmt )
{
    HSQPDELETE  hDelete         = hStmt->hStmtExtras->hParsedSQL->h.hDelete;
    HTABLE      hTable          = 0;
    HCOLUMNS    hColumns        = 0;
    long        nCols           = 0;
    long        nCol            = 0;
    HROWS       hRows           = 0;
    HROW        hRow            = 0;
    long        nRows           = 0;
    long        nRow            = 0;

    HSQPCOMPARISON  hSQPComparison;

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "START" );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, hDelete->pszTable );

    if ( !IOTableOpen( &hTable, hStmt, hDelete->pszTable, IODelete ) )
    {
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_ERROR, "Could not open table." );
		return SQL_ERROR;
    }

    /*
     * STEP 1
     *
     * Get all column headers.
     *
     */
    if ( !IOTableHeaderRead( hTable, &hColumns, &nCols ) )
    {
        IOTableClose( &hTable );
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_ERROR, "Could not read table info." );
		return SQL_ERROR;
    }   

    /*
     * STEP 2
     *
     * Get desired rows.
     *
     * - we assume we have an AND between each
     *   comparison in our WHERE (no OR)
     *
     */

    /* xref columns in WHERE */
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "xref-ing SELECT columns into interim column headers." );
    IOXrefWhere( hDelete->hWhere, hColumns, nCols );

    /* gather rows */
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "Reading desired rows into interim data set." );
    hStmt->nRowsAffected    = 0;
    while ( IOTableRead( hTable, &hRow, nCols ) )
    {
        if ( IOWhere( hRow, hDelete->hWhere, ((HDRVDBC)(hStmt->hDbc))->hDbcExtras ) )
        {
            FreeRow_( &hRow, nCols );
            hStmt->nRowsAffected++;
        }
        else
        {
            nRows++;
            hRows               = (HROWS)realloc( hRows, sizeof(HROW) * nRows );
            hRows[nRows - 1]    = hRow;
        }
    }
    sprintf((char*) hStmt->szSqlMsg, "Found %ld rows to keep and %ld rows to remove.", nRows, hStmt->nRowsAffected );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO,(char*) hStmt->szSqlMsg );

    /*
     * STEP 3
     *
     * Replace file with what we have loaded.
     *
     */
    if ( !IOTableHeaderWrite( hTable, hColumns, nCols ) )
    {
        IOTableClose( &hTable );
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_ERROR, "Could not write table info." );
        FreeColumns_( &hColumns, nCols );
        FreeRows_( &hRows, nRows, nCols );    
		return SQL_ERROR;
    }   
    for ( nRow=0; nRow < nRows; nRow++ )
    {
        IOTableWrite( hTable, hRows[nRow], nCols );
    }

    /*
     * STEP 5
     *
     * Finalize
     *
     */
    IOTableClose( &hTable );
    FreeColumns_( &hColumns, nCols );
    FreeRows_( &hRows, nRows, nCols );    

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END" );

    return SQL_SUCCESS;
}

SQLRETURN IOCreateTable( HDRVSTMT hStmt )
{
    HSQPCREATETABLE hCreateTable    = hStmt->hStmtExtras->hParsedSQL->h.hCreateTable;
    HTABLE          hTable          = 0;
    HSQPCOLUMNDEF   hColumnDef;
    HCOLUMNS        hColumns        = 0;
    long            nCols           = 0;

    if ( !hStmt )
		return SQL_ERROR;

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, hCreateTable->pszTable );

    /* 
     * INIT
     *
     */
    if ( !IOTableOpen( &hTable, hStmt, hCreateTable->pszTable, IOCreate ) )
    {
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_ERROR, "Could not open table." );
		return SQL_ERROR;
    }

    /*
     * CREATE HCOLUMNs FROM HSQPCOLUMNDEFs
     *
     * - we only support default defs right now... push this short-coming down into text-file specific IO
     *
     */
    lstFirst( hCreateTable->hColumnDefs );
    while ( !lstEOL( hCreateTable->hColumnDefs ) )
    {
        nCols++;
        hColumnDef          = (HSQPCOLUMNDEF)lstGet( hCreateTable->hColumnDefs );
        hColumns            = (HCOLUMNS)realloc( hColumns, sizeof(HCOLUMN) * nCols );
        hColumns[nCols-1]   = CreateColumn_( hCreateTable->pszTable, hColumnDef->pszColumn, DEFAULTCOLUMNTYPE, DEFAULTCOLUMNLENGTH, DEFAULTCOLUMNPRECISION );

        lstNext( hCreateTable->hColumnDefs );
    }

    /*
     * WRITE TABLE HEADER
     *
     */
    if ( !IOTableHeaderWrite( hTable, hColumns, nCols ) )
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_ERROR, "Could not write table header." );

    /*
     * FINI
     *
     */
    IOTableClose( &hTable );

    FreeColumns_( &hColumns, nCols );
    
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "SQL_SUCCESS" );

    return SQL_SUCCESS;
}

SQLRETURN IODropTable  ( HDRVSTMT hStmt )
{
    HSQPDROPTABLE   hDropTable  = hStmt->hStmtExtras->hParsedSQL->h.hDropTable;
    HTABLE          hTable      = 0;

    /* 
     * INIT
     *
     */
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "START" );
    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, hDropTable );
    if ( !IOTableOpen( &hTable, hStmt, hDropTable, IODrop ) )
    {
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_ERROR, "Could not open table." );
		return SQL_ERROR;
    }

    /* 
     * FINI
     *
     */
    if ( !IOTableClose( &hTable ) )
    {
		logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_WARNING, LOG_ERROR, "Failed to drop table." );
		return SQL_ERROR;
    }

    logPushMsg( hStmt->hLog, __FILE__, __FILE__, __LINE__, LOG_INFO, LOG_INFO, "END" );
    return SQL_SUCCESS;
}


