#include "driver.h"


HCOLUMN CreateColumn_( char *szTable, char *szName, short nType, short nLength, short nPrecision )
{
    HCOLUMN hColumn = (HCOLUMN)calloc( 1, sizeof(COLUMN) );

    hColumn->nLength    = nLength;
    hColumn->nPrecision = nPrecision;
    hColumn->nType      = nType;
    hColumn->pszName    = strdup( szName );
    hColumn->pszTable   = strdup( szTable );

    return hColumn;
}

HROW CreateRow_( int nCols, ... )
{
    va_list pArgs;
    char    *p;
    int     nCol    = 0;
    HROW    hRow    = (HROW)calloc( nCols, sizeof(HVALUE) * nCols );

    va_start( pArgs, nCols );
    for ( nCol = 0; nCol < nCols; nCol++ )
    {
        hRow[nCol] = strdup( va_arg( pArgs, HVALUE ) );
    }

    return hRow;
}

void FreeRow_( HROW *phRow, long nCols )
{
    long nCol = 0;

    if ( !*phRow )
        return;

    for ( ; nCol < nCols; nCol++ )
    {
        if ( (*phRow)[nCol] )
            free( (*phRow)[nCol] );
    }
    free( *phRow );
    *phRow = 0;
}

void FreeRows_( HROWS *phRows, long nRows, long nCols )
{
    long nRow = 0;

    if ( !*phRows )
        return;

    for ( ; nRow < nRows; nRow++ )
    {
        FreeRow_( &((*phRows)[nRow]), nCols );
    }
    free( *phRows );
    *phRows = 0;
}

void FreeColumn_( HCOLUMN *phColumn )
{
    if ( !*phColumn )
        return;

    if ( (*phColumn)->pszName )
        free( (*phColumn)->pszName );
    if ( (*phColumn)->pszTable )
        free( (*phColumn)->pszTable );

    free( *phColumn );
    *phColumn = 0;
}

void FreeColumns_( HCOLUMNS *phColumns, long nCols )
{
    long nCol = 0;

    if ( !*phColumns )
        return;

    for ( ; nCol < nCols; nCol++ )
    {
        FreeColumn_( &((*phColumns)[nCol]) );
    }
    free( *phColumns );
    *phColumns = 0;
}


