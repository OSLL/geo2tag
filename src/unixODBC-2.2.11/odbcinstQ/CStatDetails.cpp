#include "CStatDetails.h"

#include "info.xpm"

CStatDetails::CStatDetails( QWidget* parent, const char* name )
	: QWidget( parent, name, 0 )
{
    QGridLayout * pLayoutTop    = new QGridLayout( this, 1, 1 );

    // Table
    pTable = new QTable( MAXPROCESSES, MAXHANDLES+1, this );
    pTable->setLeftMargin( 0 );
    pTable->verticalHeader()->hide();

    QHeader * pHeader = pTable->horizontalHeader();
    pHeader->setLabel( 0, "PID" );
    pHeader->setLabel( 1, "Environments" );
    pHeader->setLabel( 2, "Connections" );
    pHeader->setLabel( 3, "Statements" );
    pHeader->setLabel( 4, "Descriptors" );
	    
    pLayoutTop->addWidget( pTable, 0, 0 );

	resize( 400,300 );
	setMinimumSize( 0, 0 );
	setMaximumSize( 32767, 32767 );

    // SHARED MEM
    hStats = 0;

    // TIMER
    pTimer = new QTimer( this );
    connect( pTimer, SIGNAL(timeout()), SLOT(showStats()) );
    pTimer->start( 700, FALSE );
}

CStatDetails::~CStatDetails()
{
    uodbc_close_stats( hStats );
}


void CStatDetails::showStats()
{
    int nPIDs       = 0;
    int nPID        = 0;
    int nHandles    = 0;
    int nHandle     = 0;
    QString qs;

    if ( this->isVisible() )
    {
        if ( !hStats )
            if ( uodbc_open_stats( &hStats, UODBC_STATS_READ ) != 0 );

        nPIDs = uodbc_get_stats( hStats, 0, aPIDs, MAXPROCESSES );

        for ( nPID = 0; nPID < MAXPROCESSES; nPID++ )
        {
            if ( nPID < nPIDs )
            {
                nHandles = uodbc_get_stats( hStats, aPIDs[nPID].value.l_value, aHandles, MAXHANDLES );
                if ( nHandles > 0 )
                {
                    qs.sprintf( "%d", aPIDs[nPID].value.l_value );
                    pTable->setText( nPID, 0, qs );
                    for ( nHandle = 0; nHandle < MAXHANDLES; nHandle++ )
                    {
                        qs.sprintf( "%d", aHandles[nHandle].value.l_value );
                        pTable->setText( nPID, nHandle + 1, qs );
                    }
                }
                else
                    clearRow( nPID );
            }
            else
                clearRow( nPID );
        }
    }
/*
    {
        char szError[501];
        szError[0] = '\0';
        uodbc_stats_error( szError, sizeof(szError)-1 );
    }
*/    
}

void CStatDetails::clearRow( int nRow )
{
    int nCol;

    for ( nCol = 0; nCol < pTable->numCols(); nCol++ )
    {
        pTable->setText( nRow, nCol, "" );
    }
}
