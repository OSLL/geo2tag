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
#include "classDataSource.h"

#include "computergreen.xpm"
#include "computerred.xpm"

classDataSource::classDataSource( QListView *pParent, classCanvas *pCanvas, int nDataSourceType, char *pszDataSourceName, SQLHENV hEnv )
    : classNode( pParent, pCanvas )
{
	Init( nDataSourceType,	pszDataSourceName, hEnv );
}

classDataSource::classDataSource( QListViewItem *pParent, classCanvas *pCanvas, int nDataSourceType, char *pszDataSourceName, SQLHENV hEnv )
    : classNode( pParent, pCanvas )
{
	Init( nDataSourceType,	pszDataSourceName, hEnv );
}

classDataSource::classDataSource( QListViewItem *pParent,QListViewItem *pAfter, classCanvas *pCanvas, int nDataSourceType, char *pszDataSourceName, SQLHENV hEnv )
    : classNode( pParent, pAfter, pCanvas )
{
	Init( nDataSourceType,	pszDataSourceName, hEnv );
}

classDataSource::~classDataSource()
{
	if ( pConnectionFrame )
		delete pConnectionFrame;

	if ((void*) hDbc != 0 )
	{
		if ( bLoggedIn )
			SQLDisconnect( hDbc );
		SQLFreeConnect( hDbc );
	}
}

void classDataSource::Init( int nDataSourceType, char *pszDataSourceName, SQLHENV hEnv )
{
	char	szResults[9600];
	char	szDescription[INI_MAX_PROPERTY_VALUE+1];
	char	szDriver[INI_MAX_PROPERTY_VALUE+1];
	QString	qsDescription;

	if ( nDataSourceType == classDataSource::System )
		this->nDataSourceType = nDataSourceType;
	else
		this->nDataSourceType = classDataSource::User;

	qsDataSourceName = "";
	if ( pszDataSourceName != 0 )
		qsDataSourceName = pszDataSourceName;

	this->pCanvas 	= pCanvas;
	bLoggedIn		= FALSE;
	this->hEnv		= hEnv;
	hDbc 			= (SQLHDBC)0;
	setPixmap( 0, QPixmap( computerred_xpm ) );

	// CREATE AN ODBC CONNECTION
	if ( SQLAllocConnect( hEnv, &hDbc ) != SQL_SUCCESS )
	{
		QMessageBox::critical( (QWidget *)this, "Data Manager", "Call to the ODBC Driver Manager failed" );
		return;
	}

	szResults[0] 		= '\0';
    szDescription[0]	= '\0';
    szDriver[0]			= '\0';
	if ( nDataSourceType == User )
		SQLSetConfigMode( ODBC_USER_DSN );
	else
		SQLSetConfigMode( ODBC_SYSTEM_DSN );
	if ( SQLGetPrivateProfileString((char*) qsDataSourceName.data(), "Description", "", szResults, 9600, 0 ) > 0 )
		iniElement( szResults, '\0', '\0', 0, szDescription, INI_MAX_PROPERTY_VALUE );
	if ( SQLGetPrivateProfileString((char*) qsDataSourceName.data(), "Driver", "", szResults, 9600, 0 ) > 0 )
		iniElement( szResults, '\0', '\0', 0, szDriver, INI_MAX_PROPERTY_VALUE );
	SQLSetConfigMode( ODBC_BOTH_DSN );

	qsDescription.sprintf( "%s [%s]", szDescription, szDriver );
	setText( 0, qsDataSourceName );
    setText( 1, "" );
	setText( 2, qsDescription );
	pTables = 0;
	pConnectionFrame	= 0;
}

void classDataSource::setOpen( bool bOpen )
{
    listView()->setSelected( listView()->selectedItem(), false );
    if ( bOpen && !bLoggedIn )
    {
        // try to connect
		if ( hDbc )
		{
            classLogin	*pLogin = new classLogin( pCanvas, hDbc, (char*)qsDataSourceName.data(), nDataSourceType );
			if ( pLogin->exec() )
			{
				bLoggedIn = true;
				setPixmap( 0, QPixmap( computergreen_xpm ) );
				pTables = new classTables( this, pCanvas, hDbc );
                QListViewItem::setOpen( bOpen );
			}
            else
            {
                delete pLogin;
                return;
            }
            delete pLogin;
		}
    }
    else if ( !bOpen && bLoggedIn )
    {
        // disconnect
        delete pTables;
        pTables = 0;
        delete pConnectionFrame;
        pConnectionFrame = 0;
        SQLDisconnect( hDbc );
        setPixmap( 0, QPixmap( computerred_xpm ) );
        bLoggedIn = false;
        QListViewItem::setOpen( bOpen );
    }
}

void classDataSource::setup()
{
    setExpandable( TRUE );
    QListViewItem::setup();
}

void classDataSource::selectionChanged( QListViewItem *p )
{
    if ( pTables ) pTables->selectionChanged( p );
	if ( p == this )
	{
		if ( bLoggedIn )
		{
			if ( pConnectionFrame )
				pConnectionFrame->show();
			else
			{
				pConnectionFrame = new classConnectionFrame( hDbc, qsDataSourceName, pCanvas );
				pConnectionFrame->show();
			}
		}
	}
	else if ( pConnectionFrame )
		pConnectionFrame->hide();
}



