/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 18.FEB.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/
#include "classDataManager.h"
#include "ODBC.xpm"

classDataManager::classDataManager( QWidget *pParent, const char *pszName )
	: QWidget( pParent, pszName ) //, 503808 )
{
	setCaption( "DataManager " VERSION );

	setIcon( QPixmap( xpmODBC ) );

	splSplitter = new QSplitter( this, "splSplitter" );

	lvwBrowser = new QListView( splSplitter, "lvwBrowser" );
	lvwBrowser->setMinimumSize( 50, 50 );
	lvwBrowser->setMaximumSize( 32767, 32767 );
	connect( lvwBrowser, SIGNAL(rightButtonClicked(QListViewItem*,const QPoint&,int)), SLOT(ItemMenu(QListViewItem*,const QPoint&,int)) );
	connect( lvwBrowser, SIGNAL(selectionChanged(QListViewItem*)), SLOT(ItemChanged(QListViewItem*)) );
	lvwBrowser->setFocusPolicy( QWidget::TabFocus );
	lvwBrowser->setBackgroundMode( QWidget::PaletteBackground );
	lvwBrowser->setFrameStyle( 17 );
	lvwBrowser->setResizePolicy( QScrollView::Manual );
	lvwBrowser->setVScrollBarMode( QScrollView::Auto );
	lvwBrowser->setHScrollBarMode( QScrollView::Auto );
	lvwBrowser->setTreeStepSize( 20 );
	lvwBrowser->setMultiSelection( FALSE );
	lvwBrowser->setAllColumnsShowFocus( TRUE );
	lvwBrowser->setItemMargin( 1 );
	lvwBrowser->setRootIsDecorated( TRUE );
	lvwBrowser->addColumn( "Object", -1 );
	lvwBrowser->setColumnWidthMode( 0, QListView::Maximum );
	lvwBrowser->setColumnAlignment( 0, 1 );
	lvwBrowser->addColumn( "Type", -1 );
	lvwBrowser->setColumnWidthMode( 1, QListView::Maximum );
	lvwBrowser->setColumnAlignment( 1, 1 );
	lvwBrowser->addColumn( "Description", -1 );
	lvwBrowser->setColumnWidthMode( 2, QListView::Maximum );
	lvwBrowser->setColumnAlignment( 2, 1 );
	lvwBrowser->setSorting( -1, 0 );
    lvwBrowser->setSelectionMode( QListView::Single );

	pCanvas = new classCanvas( splSplitter, "fraProperties" );

    LoadState();
	setMinimumSize( 0, 0 );
	setMaximumSize( 32767, 32767 );

	pODBC = new classODBC( lvwBrowser, pCanvas );

}

classDataManager::~classDataManager()
{
    SaveState();
}

void classDataManager::ItemMenu(QListViewItem *pItem, const QPoint &, int )
{

    if ( pItem == 0 ) return;
    lvwBrowser->setSelected( pItem, TRUE );

}

void classDataManager::ItemChanged(QListViewItem *pItem )
{
    QString         qsType;

    if ( pItem == 0 ) return;
    lvwBrowser->setSelected( pItem, TRUE );
	pODBC->selectionChanged( pItem );
}

void classDataManager::Exit()
{
    exit( 0 );
}

void classDataManager::About()
{

}

void classDataManager::resizeEvent( QResizeEvent * )
{
    int marg = 1;
    int w = width()  - marg * 2;
    int h = height() - marg * 2;

    splSplitter->setGeometry( marg, marg, w, h );
}



void classDataManager::LoadState()
{
	HINI hIni;
	char szObject[INI_MAX_OBJECT_NAME+1];
	char szPropertyName[INI_MAX_PROPERTY_NAME+1];
	char szPropertyValue[INI_MAX_PROPERTY_VALUE+1];
    char szINI[ODBC_FILENAME_MAX+1];

    sprintf( szINI, "%s/%s", QDir::homeDirPath().ascii(), ".DataManager/DataManager.ini" ); 
	if ( iniOpen( &hIni, szINI, "#;", '[', ']', '=', TRUE ) != INI_ERROR )
	{
		iniObjectFirst( hIni );
		while ( iniObjectEOL( hIni ) == FALSE )
		{
			szObject[0]         = '\0';
			szPropertyName[0] 	= '\0';
			szPropertyValue[0] 	= '\0';
			iniObject( hIni, szObject );
			iniPropertyFirst( hIni );
            if ( strcmp( szObject, "GEOMETRY" ) == 0 )
            {
                int nX = x();
                int nY = y();
                int nW = width();
                int nH = height();
                int nCell1 = 0;
                int nCell2 = 0;

    			while ( iniPropertyEOL( hIni ) == FALSE )
    			{
    				iniProperty( hIni, szPropertyName );
    				if ( strncmp( szPropertyName, "X", INI_MAX_PROPERTY_NAME ) == 0 )
                    {
    					iniValue( hIni, szPropertyValue );
                        nX = atoi( szPropertyValue );
                    }
    				if ( strncmp( szPropertyName, "Y", INI_MAX_PROPERTY_NAME ) == 0 )
                    {
    					iniValue( hIni, szPropertyValue );
                        nY = atoi( szPropertyValue );
                    }
    				if ( strncmp( szPropertyName, "W", INI_MAX_PROPERTY_NAME ) == 0 )
                    {
    					iniValue( hIni, szPropertyValue );
                        nW = atoi( szPropertyValue );
                    }
    				if ( strncmp( szPropertyName, "H", INI_MAX_PROPERTY_NAME ) == 0 )
                    {
    					iniValue( hIni, szPropertyValue );
                        nH = atoi( szPropertyValue );
                    }
    				if ( strncmp( szPropertyName, "W_CELL1", INI_MAX_PROPERTY_NAME ) == 0 )
                    {
    					iniValue( hIni, szPropertyValue );
                        nCell1 = atoi( szPropertyValue );
                    }
    				if ( strncmp( szPropertyName, "W_CELL2", INI_MAX_PROPERTY_NAME ) == 0 )
                    {
    					iniValue( hIni, szPropertyValue );
                        nCell2 = atoi( szPropertyValue );
                    }

    				iniPropertyNext( hIni );
    			}
                resize( nW, nH );
                move( nX, nY );
                if ( nCell1 && nCell2 )
                {
                    QValueList<int> list;
                    list.append( nCell1 );
                    list.append( nCell2 );
                    splSplitter->setSizes( list );
                }
            }
			iniObjectNext( hIni );
		}
	}
	else
	{
		QMessageBox::warning(	this, "DataManager",  "Could not open ~/.DataManager/DataManager.ini.\nYou may not have a home dir." );
	}
}

void classDataManager::SaveState()
{
	HINI hIni;
	char szPropertyValue[INI_MAX_PROPERTY_VALUE+1];
    char szINI[ODBC_FILENAME_MAX+1];

    sprintf( szINI, "%s/%s", QDir::homeDirPath().ascii(), ".DataManager/DataManager.ini" ); 
	if ( iniOpen( &hIni, szINI, "#;", '[', ']', '=', TRUE ) != INI_ERROR )
	{
        // REMOVE OBJECT
		iniObjectSeekSure( hIni, "GEOMETRY" );
        iniObjectDelete( hIni );

        // ADD OBJECT
		iniObjectSeekSure( hIni, "GEOMETRY" );
        sprintf( szPropertyValue, "%d", pos().x() );
        iniPropertyInsert( hIni, "X", szPropertyValue );

        sprintf( szPropertyValue, "%d", pos().y() );
        iniPropertyInsert( hIni, "Y", szPropertyValue );

        sprintf( szPropertyValue, "%d", size().width() );
        iniPropertyInsert( hIni, "W", szPropertyValue );

        sprintf( szPropertyValue, "%d", size().height() );
        iniPropertyInsert( hIni, "H", szPropertyValue );

        sprintf( szPropertyValue, "%d", splSplitter->sizes().first() );
        iniPropertyInsert( hIni, "W_CELL1", szPropertyValue );

        sprintf( szPropertyValue, "%d", splSplitter->sizes().last() );
        iniPropertyInsert( hIni, "W_CELL2", szPropertyValue );

        iniCommit( hIni );
	}
}


