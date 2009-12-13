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
#include "classDrivers.h"
#include "drivers.xpm"

classDrivers::classDrivers( QListView *pParent, classCanvas *pCanvas, SQLHENV	hEnv )
    : classNode( pParent, pCanvas )
{
	Init( hEnv );
}

classDrivers::classDrivers( QListViewItem *pParent, classCanvas *pCanvas, SQLHENV	hEnv )
    : classNode( pParent, pCanvas )
{
	Init( hEnv );
}

classDrivers::classDrivers( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHENV	hEnv )
    : classNode( pParent, pAfter, pCanvas )
{
	Init( hEnv );
}

classDrivers::~classDrivers()
{
	listDrivers.clear();
}
void classDrivers::Init( SQLHENV	hEnv )
{
	this->pCanvas 	= pCanvas;
	this->hEnv 		= hEnv;
	listDrivers.setAutoDelete( TRUE );
	setText( 0, "Drivers" );
	setText( 1, "" );
    setText( 2, "" );
	setPixmap( 0, QPixmap( drivers_xpm ) );
}

/*******************************************************
 * setOpen
 *******************************************************
 *
 *
 *******************************************************/
void classDrivers::setOpen( bool o )
{
	SQLRETURN		nReturn;
	SQLCHAR			szDriverDesc[1024];

	/********************************
	 * METHOD 1 - Using Driver Manager
	 ********************************/
    if ( o && !childCount() )
	{
		nReturn = SQLDrivers( hEnv, SQL_FETCH_FIRST, szDriverDesc, sizeof(szDriverDesc)-1, 0, 0, -1, 0 );	
		while ( nReturn == SQL_SUCCESS )
		{
			listDrivers.append( new classDriver( this, pCanvas, (char*)szDriverDesc, hEnv ) );
			nReturn = SQLDrivers( hEnv, SQL_FETCH_NEXT, szDriverDesc, sizeof(szDriverDesc)-1, 0, 0, -1, 0 );	
		}
	}
	QListViewItem::setOpen( o );

	/********************************
	 * METHOD 2 - Using odbcinst
	 ********************************/
	/*
	int					nElement;
	char 				szResults[9600];
	char				szObjectName[INI_MAX_OBJECT_NAME+1];

    if ( o && !childCount() )
	{
		// ADD CHILD NODES; only classDataSources knows what they may be
		szResults[0] = '\0';
		if ( SQLGetInstalledDrivers( szResults, 9600, 0 ) == TRUE )
		{
			for ( nElement = 0; iniElement( szResults, '\0', '\0', nElement, szObjectName, INI_MAX_OBJECT_NAME ) == INI_SUCCESS; nElement++ )
			{
				listDrivers.append( new classDriver( this, pCanvas, szObjectName, hEnv ) );
			}
		}
		else
		{
			//SQLInstallerError( 1, &nError, szError, FILENAME_MAX, 0 );
			//if ( cVerbose == 0 ) printf( "[odbcinst] SQLGetPrivateProfileString failed with %s.\n", szError );
			//return;
		}
	}
    QListViewItem::setOpen( o );
	*/
}

void classDrivers::setup()
{
    setExpandable( TRUE );
    QListViewItem::setup();
}


void classDrivers::selectionChanged( QListViewItem *p )
{
	classDriver	*pDriver;

	for ( pDriver = listDrivers.first(); pDriver != 0; pDriver = listDrivers.next() )
        pDriver->selectionChanged( p );

	if ( p == this )
	{
	}
}

