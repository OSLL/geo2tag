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
#include "classDriver.h"
#include "driver.xpm"

classDriver::classDriver( QListView *pParent, classCanvas *pCanvas, char *pszDriverName, SQLHENV	hEnv  )
    : classNode( pParent, pCanvas )
{
	Init( pszDriverName, hEnv );
}

classDriver::classDriver( QListViewItem *pParent, classCanvas *pCanvas, char *pszDriverName, SQLHENV	hEnv )
    : classNode( pParent, pCanvas )
{
	Init( pszDriverName, hEnv );
}

classDriver::classDriver( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, char *pszDriverName, SQLHENV	hEnv )
    : classNode( pParent, pAfter, pCanvas )
{
	Init( pszDriverName, hEnv );
}

classDriver::~classDriver()
{
}

/*******************************************************
 * Init
 *******************************************************
 * Call the odbcinst to get Driver description.
 *
 * NOTE: 	This should be done via the Driver Manager by
 *			calling SQLDrivers() but the iODBC Driver Manager
 *			does not, at this time, adequately support this call.
 *******************************************************/
void classDriver::Init( char *pszDriverName, SQLHENV hEnv )
{
	char 				szResults[1024];
	char				szPropertyValue[INI_MAX_PROPERTY_VALUE+1];

	qsDriverName = "";
	if ( pszDriverName != 0 )
		qsDriverName = pszDriverName;

	this->pCanvas 	= pCanvas;
	this->hEnv 		= hEnv;
	setPixmap( 0, QPixmap( driver_xpm ) );

	szResults[0] 		= '\0';
    szPropertyValue[0]	= '\0';
	if ( SQLGetPrivateProfileString((char*) qsDriverName.data(), "Description", "", szResults, sizeof(szResults), "odbcinst" ) > 0 )
		iniElement( szResults, '\0', '\0', 0, szPropertyValue, INI_MAX_PROPERTY_VALUE );
	else
	{
		//SQLInstallerError( 1, &nError, szError, FILENAME_MAX, 0 );
		//if ( cVerbose == 0 ) printf( "[odbcinst] SQLGetPrivateProfileString failed with %s.\n", szError );
		//return;
	}

	setText( 0, qsDriverName );
	setText( 1, "" );
	setText( 2, szPropertyValue );
}

void classDriver::setOpen( bool o )
{
    if ( o && !childCount() )
	{
		setExpandable( FALSE );
    }
    QListViewItem::setOpen( o );
}

void classDriver::setup()
{
    setExpandable( TRUE );
    QListViewItem::setup();
}


void classDriver::selectionChanged( QListViewItem *p )
{
	if ( p == this )
	{
	}
}

