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
#include "classDataSources.h"
#include "datasourcesuser.xpm"
#include "datasourcessystem.xpm"

classDataSources::classDataSources( QListView *pParent, classCanvas *pCanvas, int nDataSourceType, SQLHENV hEnv )
    : classNode( pParent, pCanvas )
{
	Init( nDataSourceType, hEnv );
}

classDataSources::classDataSources( QListViewItem *pParent, classCanvas *pCanvas, int nDataSourceType, SQLHENV hEnv )
    : classNode( pParent, pCanvas )
{
	Init( nDataSourceType, hEnv );
}

classDataSources::classDataSources( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, int nDataSourceType, SQLHENV hEnv )
    : classNode( pParent, pAfter, pCanvas )
{
	Init( nDataSourceType, hEnv );
}

classDataSources::~classDataSources()
{
	listDataSources.clear();
}

void classDataSources::Init( int nDataSourceType, SQLHENV hEnv )
{

	switch ( nDataSourceType )
	{
	case classDataSources::System :
		this->nDataSourceType = nDataSourceType;
		setText( 0, "System Data Sources" );
		setText( 1, "" );
        setText( 2, "" );
		setPixmap( 0, QPixmap( datasourcessystem_xpm ) );
		break;
	default:
		this->nDataSourceType = classDataSources::User;
		setText( 0, "User Data Sources" );
		setText( 1, "" );
        setText( 2, "" );
		setPixmap( 0, QPixmap( datasourcesuser_xpm ) );
	}

	this->pCanvas 	= pCanvas;
	this->hEnv 		= hEnv;
	listDataSources.setAutoDelete( TRUE );

}

void classDataSources::setOpen( bool bOpen )
{
	int					nElement;
	char 				szResults[9600];
	char				szObjectName[INI_MAX_OBJECT_NAME+1];

    memset( szResults, 0, sizeof(szResults) );  // Results buffer must be cleared 
    classDataSource     *pLastDataSource    = NULL;

    if ( bOpen )
    {
		memset( szResults, 0, sizeof(szResults) );
		if ( nDataSourceType == User )
			SQLSetConfigMode( ODBC_USER_DSN );
		else
			SQLSetConfigMode( ODBC_SYSTEM_DSN );

		if ( SQLGetPrivateProfileString( 0, 0, 0, szResults, 9600, 0 ) > 0 )
		{
			for ( nElement = 0; iniElement( szResults, '\0', '\0', nElement, szObjectName, INI_MAX_OBJECT_NAME ) == INI_SUCCESS; nElement++ )
			{
				listDataSources.append( pLastDataSource = new classDataSource( this, pLastDataSource, pCanvas, nDataSourceType, szObjectName, hEnv ) );
			}
		}
		else
		{
			//SQLInstallerError( 1, &nError, szError, FILENAME_MAX, 0 );
			//if ( cVerbose == 0 ) printf( "[odbcinst] SQLGetPrivateProfileString failed with %s.\n", szError );
			//return;
		}
		SQLSetConfigMode( ODBC_BOTH_DSN );
    }
    else
    {
        listDataSources.clear();
    }
    QListViewItem::setOpen( bOpen );
}

void classDataSources::setup()
{
    setExpandable( TRUE );
    QListViewItem::setup();
}

void classDataSources::selectionChanged( QListViewItem *p )
{
	classDataSource	*pDataSource;

	for ( pDataSource = listDataSources.first(); pDataSource != 0; pDataSource = listDataSources.next() )
        pDataSource->selectionChanged( p );

	if ( p == this )
	{
	}
}


