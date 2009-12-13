#include "driver.h"

/* 
 * IO SUPPORT FUNCS
 *
 */
typedef struct
{
    HDBCEXTRAS  hDbcExtras;
    HLOG        hLog;
    char *      pszLog;
    FILE *      hFile;
    char        szTableFile[FILENAME_MAX];
    char        szTableName[FILENAME_MAX];
    long        nLine;
    IOMode      nMode;

} TXTTABLE, * HTXTTABLE;


int IOTableOpen( HTABLE *phTable, HDRVSTMT hStmt, char *szTableName, IOMode nMode )
{
    char *      FUNC_       = "IOTableOpen";
    HDRVDBC     hDbc        = (HDRVDBC)hStmt->hDbc;
    HTXTTABLE * phTXTTable  = (HTXTTABLE *)phTable;

    logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "START:" );

    /* INIT TABLE STRUCT */
    *phTXTTable                 = (HTXTTABLE)calloc( 1, sizeof(TXTTABLE) );
    (*phTXTTable)->hDbcExtras   = hDbc->hDbcExtras;
    (*phTXTTable)->nLine        = 0;
    (*phTXTTable)->hLog         = hStmt->hLog;
    (*phTXTTable)->pszLog       = (char*)hStmt->szSqlMsg;
    (*phTXTTable)->nMode        = nMode;


    sprintf( (*phTXTTable)->szTableFile, "%s/%s", hDbc->hDbcExtras->pszDirectory, szTableName );
    strncpy( (*phTXTTable)->szTableName, szTableName, FILENAME_MAX - 1 );

    /* OPEN FILE */
    if ( nMode == IOCreate )
    {
        /* FAIL IF TABLE ALREADY EXISTS */
        if ( access( (*phTXTTable)->szTableFile, F_OK ) == 0 )
        {
            sprintf((char*) hStmt->szSqlMsg, "Table [%s] already exists.", (*phTXTTable)->szTableFile );
            logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hStmt->szSqlMsg );
            free( *phTXTTable );
            return 0;
        }
        (*phTXTTable)->hFile    = fopen( (*phTXTTable)->szTableFile, "w+" );
    }
    else 
    {
        (*phTXTTable)->hFile    = fopen( (*phTXTTable)->szTableFile, "r+" );
	/* if it did not open it may need a txt extension */
        if ( !(*phTXTTable)->hFile ) {
	        sprintf( (*phTXTTable)->szTableFile, "%s/%s.%s", hDbc->hDbcExtras->pszDirectory, szTableName, "txt" );
	        strncpy( (*phTXTTable)->szTableName, szTableName, FILENAME_MAX - 1 );
        	(*phTXTTable)->hFile    = fopen( (*phTXTTable)->szTableFile, "r+" );
	}
	/* if it did not open it may need a csv extension */
        if ( !(*phTXTTable)->hFile ) {
	        sprintf( (*phTXTTable)->szTableFile, "%s/%s.%s", hDbc->hDbcExtras->pszDirectory, szTableName, "csv" );
	        strncpy( (*phTXTTable)->szTableName, szTableName, FILENAME_MAX - 1 );
        	(*phTXTTable)->hFile    = fopen( (*phTXTTable)->szTableFile, "r+" );
	}
    }
    if ( !(*phTXTTable)->hFile )
    {
	/* put back the original name to prevent confusion on the users part */
        sprintf( (*phTXTTable)->szTableFile, "%s/%s", hDbc->hDbcExtras->pszDirectory, szTableName );
        strncpy( (*phTXTTable)->szTableName, szTableName, FILENAME_MAX - 1 );
        sprintf((char*) hStmt->szSqlMsg, "Could not open [%s].", (*phTXTTable)->szTableFile );
        logPushMsg( hStmt->hLog, __FILE__, FUNC_, __LINE__, LOG_WARNING, LOG_WARNING,(char*) hStmt->szSqlMsg );
        free( *phTXTTable );
        *phTXTTable = 0;
        return 0;
    }
     /* PREPARE FOR INSERT AS REQUIRED */
    if ( nMode == IOInsert )
        fseek( (*phTXTTable)->hFile, 0, SEEK_END );

    logPushMsg( (*phTXTTable)->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "END:" );

    return 1;
}

int IOTableClose( HTABLE *phTable )
{
    char *      FUNC_       = "IOTableClose";
    HTXTTABLE * phTXTTable  = (HTXTTABLE *)phTable;
    int         bRetVal     = 1;

    sprintf( (*phTXTTable)->pszLog, "START: %s", (*phTXTTable)->szTableFile );
    logPushMsg( (*phTXTTable)->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, (*phTXTTable)->pszLog );

    fclose( (*phTXTTable)->hFile );

    if ( (*phTXTTable)->nMode == IODrop )
    {
        sprintf( (*phTXTTable)->pszLog , "Removing [%s].", (*phTXTTable)->szTableFile );
        logPushMsg( (*phTXTTable)->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, (*phTXTTable)->pszLog );

        if ( unlink( (*phTXTTable)->szTableFile ) )
            bRetVal = 0;
    }

    logPushMsg( (*phTXTTable)->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "END" );

/* We have adopted the Stmt log so do not close - PAH */
/*    logClose( (*phTXTTable)->hLog ); */

    free( *phTXTTable );
    *phTXTTable = 0;

    return bRetVal;
}

int IOTableHeaderRead( HTABLE hTable, HCOLUMNS *phColumns, long *pnCols )
{
    char      * FUNC_       = "IOTableHeaderRead";
    HTXTTABLE   hTXTTable   = (HTXTTABLE)hTable;
    long        nFilePos    = 0;
    long        nCol        = 0;
    long        nCols       = 0;
    HCOLUMNS    hColumns    = 0;
    HVALUE      hValue      = 0;
    int         cChar;
    int         nChar       = 0;
    char        szName[MAXCOLUMNNAME_LEN + 1];
    int         bRetVal     = 1;

    sprintf( hTXTTable->pszLog, "START: %s", hTXTTable->szTableFile );
    logPushMsg( hTXTTable->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, hTXTTable->pszLog );

    /* INIT */
    nFilePos = ftell( hTXTTable->hFile );
    rewind( hTXTTable->hFile );

    /* values until Nl || eof */
    while ( 1 )
    {
        cChar = fgetc( hTXTTable->hFile );

        /* EOF just where we expect it to be */
        if ( cChar == EOF && !hValue )
            break;

        /* end of column */
        if ( cChar == '\n' || cChar == hTXTTable->hDbcExtras->cColumnSeperator || cChar == EOF )
        {
            hValue        = (HVALUE)realloc( hValue, sizeof(char) * (nChar + 1) );
            hValue[nChar] = '\0';

            nCol++;
            nCols++;
            if ( *hValue )
                strncpy( szName, hValue, MAXCOLUMNNAME_LEN );
            else
                sprintf( szName, DEFAULTCOLUMNNAME, nCol ); 
            hColumns            = realloc( hColumns, sizeof(HCOLUMN) * nCols );
            hColumns[nCol - 1]  = CreateColumn_( hTXTTable->szTableName, szName, SQL_VARCHAR, DEFAULTCOLUMNLENGTH, DEFAULTCOLUMNPRECISION );
            logPushMsg( hTXTTable->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, szName );

            free( hValue );

            nChar     = 0;
            hValue    = 0;

            if ( cChar == '\n' || cChar == EOF )
                break;
        }
        /* value char */
        else if ( nChar < DEFAULTCOLUMNLENGTH && cChar != '\r' )
        {
            hValue          = (HVALUE)realloc( hValue, sizeof(char) * (nChar + 1) );
            hValue[nChar]   = cChar;
            nChar++;
        }
    } /* while */


    /* FINI */
    fseek( hTXTTable->hFile, nFilePos, SEEK_SET );

    if ( !nCols )
        bRetVal = 0;
    else
    {
        *pnCols     = nCols;
        *phColumns  = hColumns;
    }

    logPushMsg( hTXTTable->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "END:" );

    return 1;
}

int IOTableHeaderWrite( HTABLE hTable, HCOLUMNS hColumns, long nCols )
{
    char *      FUNC_       = "IOTableHeaderWrite";
    HTXTTABLE   hTXTTable   = (HTXTTABLE)hTable;
    long        nCol        = 0;

    logPushMsg( hTXTTable->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "START:" );

    fclose( hTXTTable->hFile );
    hTXTTable->hFile = fopen( hTXTTable->szTableFile, "w" );

    for ( ; nCol < nCols; nCol++ )
    {
        fputs( hColumns[nCol]->pszName, hTXTTable->hFile );
        if ( nCol < nCols - 1 )
            fputc( hTXTTable->hDbcExtras->cColumnSeperator, hTXTTable->hFile );
    }
    fputc( '\n', hTXTTable->hFile );

    logPushMsg( hTXTTable->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "END:" );

    return 1;
}

int IOTableRead( HTABLE hTable, HROW *phRow, long nCols )
{
    char *      FUNC_       = "IOTableRead";
    HTXTTABLE   hTXTTable = (HTXTTABLE)hTable;
    HROW        hRow        = 0;
    HVALUE      hValue      = 0;
    long        nCol        = 0;
    int         cChar;
    int         nChar       = 0;
    int         nEsc        = 0;

    sprintf( hTXTTable->pszLog, "START: %s", hTXTTable->szTableFile );
    logPushMsg( hTXTTable->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, hTXTTable->pszLog );

    /* skip first row... it is a column header */
    if ( !ftell( hTXTTable->hFile ) )
    {
        while ( 1 )
        {
            cChar = fgetc( hTXTTable->hFile );
            if ( cChar == EOF || cChar == '\n' )
            {
                if ( cChar == '\n' )
                    hTXTTable->nLine++;
                break;
            }
        }
    }

    /* read one row */
    while ( 1 )
    {
        cChar = fgetc( hTXTTable->hFile );

        /* EOF just where we expect it to be */
        if ( cChar == EOF && !hRow && !hValue )
            break;

        /* end of column */
        if ( cChar == '\n' || ( cChar == hTXTTable->hDbcExtras->cColumnSeperator && !nEsc ) || cChar == EOF )
        {
            hValue        = (HVALUE)realloc( hValue, sizeof(char) * (nChar + 1) );
            hValue[nChar] = '\0';

            nCol++;

            /* ignore extra columns that may come in some, BAD, rows */
            if ( nCol > nCols )
            {
                sprintf( hTXTTable->pszLog, "Too many columns in %s on row %ld. Truncating extra value(s).", hTXTTable->szTableFile, hTXTTable->nLine );
                logPushMsg( hTXTTable->hLog, __FILE__, FUNC_, __LINE__, LOG_WARNING, LOG_WARNING, hTXTTable->pszLog );
                free( hValue );
            }
            /* append value to row */
            else
            {
                if ( !hRow )
                    hRow = (HROW)calloc( 1, sizeof(HVALUE) * nCols );
                hRow[nCol - 1] = hValue;
            }

            nChar     = 0;
            hValue    = 0;
            nEsc      = 0;

            /* end of row */
            if ( (cChar == '\n' || cChar == EOF) && hRow )
            {
                hTXTTable->nLine++;
                /* append missing columns */
                if ( nCol < nCols )
                {
                    sprintf( hTXTTable->pszLog, "Too few columns in %s on row %ld. Adding empty value(s).", hTXTTable->szTableFile, hTXTTable->nLine );
                    logPushMsg( hTXTTable->hLog, __FILE__, FUNC_, __LINE__, LOG_WARNING, LOG_WARNING, hTXTTable->pszLog );
                    for ( ; nCol <= nCols; nCol++ )
                    {
                        hRow[nCol - 1] = (HVALUE)calloc( 1, sizeof(char) );
                    }
                }
            }

            if ( cChar == EOF || cChar == '\n' )
                break;
        }
        /* last char was an escape */
        else if ( nEsc && nChar < DEFAULTCOLUMNLENGTH )
        {
            hValue          = (HVALUE)realloc( hValue, sizeof(char) * (nChar + 1) );

            if ( cChar == '\\' )
                hValue[nChar]   = '\\';
            else if ( cChar == 'n' )
                hValue[nChar]   = '\n';
            else if ( cChar == 'r' )
                hValue[nChar]   = '\r';
            else if ( cChar == 't' )
                hValue[nChar]   = '\t';
            else if ( cChar == 'b' )
                hValue[nChar]   = '\b';
            else if ( cChar == 'f' )
                hValue[nChar]   = '\f';
            else if ( cChar == hTXTTable->hDbcExtras->cColumnSeperator )
                hValue[nChar]   = hTXTTable->hDbcExtras->cColumnSeperator;
            else
                /* ignore wrong escape */
                hValue[nChar]   = cChar;

            nChar++;
            nEsc = 0;
        }
        /* char is an escape, let's wait for next char */
        else if ( cChar == '\\' )
            nEsc = 1;
        /* value char */
        else if ( nChar < DEFAULTCOLUMNLENGTH && cChar != '\r' )
        {
            hValue          = (HVALUE)realloc( hValue, sizeof(char) * (nChar + 1) );
            hValue[nChar]   = cChar;
            nChar++;
        }
    } /* while */

    logPushMsg( hTXTTable->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "END:" );

    if ( !hRow )
        return 0;

    *phRow = hRow;

    return 1;
}

int IOTableWrite( HTABLE hTable, HROW hRow, long nCols )
{
    char *      FUNC_       = "IOTableWrite";
    HTXTTABLE   hTXTTable   = (HTXTTABLE)hTable;
    HVALUE      hValue      = 0;
    long        nCol        = 0;
    int         nChar       = 0;

    logPushMsg( hTXTTable->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "START:" );

    for ( ; nCol < nCols; nCol++ )
    {
        /* escape special chars */
        hValue = hRow[nCol];
        for ( nChar = 0 ; hValue[nChar] ; nChar++ )
        {
            if ( hValue[nChar] == '\\' )
            {
                fputc( '\\', hTXTTable->hFile );
                fputc( '\\', hTXTTable->hFile );
            }
            else if ( hValue[nChar] == '\n' )
            {
                fputc( '\\', hTXTTable->hFile );
                fputc( 'n', hTXTTable->hFile );
            }
            else if ( hValue[nChar] == '\r' )
            {
                fputc( '\\', hTXTTable->hFile );
                fputc( 'r', hTXTTable->hFile );
            }
            else if ( hValue[nChar] == '\t' )
            {
                fputc( '\\', hTXTTable->hFile );
                fputc( 't', hTXTTable->hFile );
            }
            else if ( hValue[nChar] == '\b' )
            {
                fputc( '\\', hTXTTable->hFile );
                fputc( 'b', hTXTTable->hFile );
            }
            else if ( hValue[nChar] == '\f' )
            {
                fputc( '\\', hTXTTable->hFile );
                fputc( 'f', hTXTTable->hFile );
            }
            else if ( hValue[nChar] == hTXTTable->hDbcExtras->cColumnSeperator )
            {
                fputc( '\\', hTXTTable->hFile );
                fputc( hTXTTable->hDbcExtras->cColumnSeperator, hTXTTable->hFile );
            }
            else
                fputc( hValue[nChar], hTXTTable->hFile );
        }

        if ( nCol < nCols - 1 )
            fputc( hTXTTable->hDbcExtras->cColumnSeperator, hTXTTable->hFile );
        else
            fputc( '\n', hTXTTable->hFile );
    }

    logPushMsg( hTXTTable->hLog, __FILE__, FUNC_, __LINE__, LOG_INFO, LOG_INFO, "END:" );

    return 1;
}

int IOLike( char *pRow, char *pVal, char cEscape, HDBCEXTRAS hDbcExtras )
{
    int             nNotEqual;
    int             bEqual  = 0;
    char           *pszVal;

    if ( !pRow )
        return bEqual;

    if ( !pVal )
        return bEqual;

    pszVal = (char *)calloc( 1, strlen( pVal ) );

    /* parse comparison value */
    while ( *pVal )
    {
        /* skip escape char if necessary */
        if ( ( *pVal == cEscape ) && ( *(pVal+1) == '_' || *(pVal+1) == '%' ) )
        {
            pszVal[strlen( pszVal )] = *(pVal+1);
            pVal++;
        }
        else
        /* match any character */
        if ( *pVal == '_' )
        {
            if ( hDbcExtras->bCaseSensitive )
                nNotEqual = strncmp( pRow, pszVal, strlen( pszVal ) );
            else
                nNotEqual = strncasecmp( pRow, pszVal, strlen( pszVal ) );
            if ( !nNotEqual )
            {
                if ( *(pRow + strlen( pszVal ) ) )
                {
                    pRow += strlen( pszVal ) + 1;
                    memset( pszVal, 0, sizeof( pszVal ) );
                }
                else
                {
                   /* force not match return */
                   pRow = NULL;
                   break;
                }
            }
            else
                break;
        }
        else
        /* match any string */
        if ( *pVal == '%' )
        {
            if ( hDbcExtras->bCaseSensitive )
                nNotEqual = strncmp( pRow, pszVal, strlen( pszVal ) );
            else
                nNotEqual = strncasecmp( pRow, pszVal, strlen( pszVal ) );
            if ( !nNotEqual )
            {
                pRow += strlen( pszVal );
                nNotEqual = 1;
                /* try all substrings of pRow until one match */
                do
                {
                    if ( IOLike( pRow, pVal+1, cEscape, hDbcExtras ) )
                        {
                        nNotEqual = 0;
                        break;
                        }
                }
                while ( *pRow++ );
                if ( !nNotEqual )
                    /* force match return */
                    pRow = pszVal;
                else
                    /* force not match return */
                    pRow = NULL;
                break;
            }
            else
                break;
        }
        else
            pszVal[strlen( pszVal )] = *pVal;

        pVal++;
    }
 
    if ( pRow )
        if ( hDbcExtras->bCaseSensitive )
            bEqual = !strcmp( pRow, pszVal );
        else
            bEqual = !strcasecmp( pRow, pszVal );
 
    if ( pszVal )
        free( pszVal );

    return bEqual;
}

int IOComp( HROW hRow, HSQPCOMPARISON hComp, HDBCEXTRAS hDbcExtras )
{
    int             nNotEqual;
    int             bEqual  = 0;

    if ( hComp->nLColumn < 0 )
        return bEqual;

    if ( !hComp->pszRValue )
        return bEqual;

    if ( !strcmp( hComp->pszOperator, "LIKE" ) )
        return IOLike( hRow[hComp->nLColumn], hComp->pszRValue, hComp->cEscape, hDbcExtras );
    else
    if ( !strcmp( hComp->pszOperator, "NOTLIKE" ) )
        return !IOLike( hRow[hComp->nLColumn], hComp->pszRValue, hComp->cEscape, hDbcExtras );
    else
    {
        if ( hDbcExtras->bCaseSensitive )
            nNotEqual = strcmp( hRow[hComp->nLColumn], hComp->pszRValue );
        else
            nNotEqual = strcasecmp( hRow[hComp->nLColumn], hComp->pszRValue );
        if ( nNotEqual < 0 && (hComp->pszOperator)[0] != '<' )
            return bEqual;
        if ( nNotEqual > 0 && (hComp->pszOperator)[0] != '>' && (hComp->pszOperator)[1] != '>' )
            return bEqual;
        if ( !nNotEqual && (hComp->pszOperator)[0] != '=' && (hComp->pszOperator)[1] != '=' )
            return bEqual;

        bEqual = 1;

        return bEqual;
    }
}

int IOSortCompare( HLST hOrderBy, sqpOrder nOrder, HROW hLRow, HROW hRRow )
{
    HSQPCOLUMN  hSQPColumn;
    int         nCompare    = 0;
    int         bSwap       = 0;

    lstFirst( hOrderBy );
    while ( !lstEOL( hOrderBy ) )
    {
        hSQPColumn  = (HSQPCOLUMN)lstGet( hOrderBy );
        if ( hSQPColumn->nColumn >= 0 )
        {
            nCompare  = strcmp( hLRow[hSQPColumn->nColumn], hRRow[hSQPColumn->nColumn] );
            if ( nCompare > 0 && nOrder == sqpdesc )
            {
                bSwap = 1;
                break;
            }
            if ( nCompare < 0 && nOrder == sqpasc )
            {
                bSwap = 1;
                break;
            }
        }
        lstNext( hOrderBy );
    }

    return bSwap;
}

void IOSortSwap( HROWS hRows, long n1, long n2 )
{
    HROW hRow;

    hRow        = hRows[n1];
    hRows[n1]   = hRows[n2];
    hRows[n2]   = hRow;
}

void IOSort( HLST hOrderBy, sqpOrder nOrder, HROWS hRows, long left, long right )
{
    long i;
    long last;

    if ( nOrder == sqpnone )
        return;
    if ( left >= right )
        return;

    IOSortSwap( hRows, left, (left + right) / 2 );
    last = left;
    for ( i= left + 1; i <= right; i++ )
    {
        if ( IOSortCompare( hOrderBy, nOrder, hRows[i], hRows[left] ) )
        {
            IOSortSwap( hRows, ++last, i );
        }
    }
    IOSortSwap( hRows, left, last );
    IOSort( hOrderBy, nOrder, hRows, left, last - 1 );
    IOSort( hOrderBy, nOrder, hRows, last+1, right );
}


void IOXrefWhere( HSQPCOND hWhere, HCOLUMNS hColumns, long nCols )
{
    long nCol = 0;

    if ( !hWhere)
        return;

    if ( hWhere->hComp )
    {
        hWhere->hComp->nLColumn    = -1;
        for ( nCol=0; nCol < nCols; nCol++ )
        {
            if ( !strcasecmp( hWhere->hComp->pszLValue, hColumns[nCol]->pszName ) )
            {
                hWhere->hComp->nLColumn = nCol;
                break;
            }
        }
    }
    else
    {
        IOXrefWhere( hWhere->hLCond, hColumns, nCols );
        IOXrefWhere( hWhere->hRCond, hColumns, nCols );
    }
}

int IOWhere( HROW hRow, HSQPCOND hWhere, HDBCEXTRAS hDbcExtras )
{
    if ( !hWhere)
        return 1;

    switch( hWhere->nType )
    {
    case sqpor :
        return ( IOWhere( hRow, hWhere->hLCond, hDbcExtras ) || IOWhere( hRow, hWhere->hRCond, hDbcExtras) );

    case sqpand :
        return ( IOWhere( hRow, hWhere->hLCond, hDbcExtras ) && IOWhere( hRow, hWhere->hRCond, hDbcExtras) );

    case sqpnot :
        return ( !IOWhere( hRow, hWhere->hLCond, hDbcExtras ) );

    case sqppar :
        return ( IOWhere( hRow, hWhere->hLCond, hDbcExtras ) );

    case sqpcomp :
        return ( IOComp( hRow, hWhere->hComp, hDbcExtras ) );

    default :
        return 0;
    }
}

