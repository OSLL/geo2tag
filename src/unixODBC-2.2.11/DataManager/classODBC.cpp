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
#include "classODBC.h"
#include "ODBC.xpm"

classODBC::classODBC( QListView *pParent, classCanvas *pCanvas )
    : classNode( pParent, pCanvas )
{
	Init();
}

classODBC::classODBC( QListViewItem *pParent, classCanvas *pCanvas )
    : classNode( pParent, pCanvas )
{
	Init();
}

classODBC::~classODBC()
{
	if (( void * ) hEnv != 0 )
	{
		SQLFreeEnv( hEnv );
	}
}

void classODBC::Init()
{
	hEnv 				= 0;
	this->pCanvas 		= pCanvas;
	pDrivers			= 0;
	pDataSourcesUser	= 0;
	pDataSourcesSystem	= 0;
	setPixmap( 0, QPixmap(xpmODBC) );
	setText( 0, "ODBC" );
	setText( 1, "" );
	setText( 2, "Open Database Connectivity" );
}

void classODBC::setOpen( bool o )
{
    if ( o && !childCount() )
	{
		// CREATE AN ODBC ENVIRONMENT
		if ( SQLAllocEnv( &hEnv ) != SQL_SUCCESS )
		{
			QMessageBox::critical( pCanvas, "Data Manager", "Call to the ODBC Driver Manager failed" );
			return;
		}

		// ADD CHILD NODES; only classODBC knows what they may be
		pDrivers 			= new classDrivers( this, pCanvas, hEnv );
		pDataSourcesSystem	= new classDataSources( this, pDrivers, pCanvas, classDataSources::System, hEnv );
		pDataSourcesUser	= new classDataSources( this, pDataSourcesSystem, pCanvas, classDataSources::User, hEnv );
    }
    QListViewItem::setOpen( o );
}

void classODBC::setup()
{
    setExpandable( TRUE );
    QListViewItem::setup();
}

void classODBC::selectionChanged( QListViewItem *p )
{
	if ( pDrivers ) pDrivers->selectionChanged( p );
	if ( pDataSourcesUser ) pDataSourcesUser->selectionChanged( p );
	if ( pDataSourcesSystem ) pDataSourcesSystem->selectionChanged( p );

	if ( p == this )
	{
	}

}
