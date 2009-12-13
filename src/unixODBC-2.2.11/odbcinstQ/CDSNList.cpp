/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 31.JAN.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/

#include "CDSNList.h"

#ifdef HAVE_STRNCASECMP
#ifdef HAVE_STRINGS
#include <strings.h>
#endif
#ifdef HAVE_STRING
#include <string.h>
#endif
#else
int strncasecmp( char *, char *, int );
#endif

CDSNList::CDSNList( QWidget* parent, const char* name )
	: QListView( parent, name )
{
	resize( 310,230 );
	setMinimumSize( 0, 0 );
	setMaximumSize( 32767, 32767 );
	setFocusPolicy( QWidget::TabFocus );
	setBackgroundMode( QWidget::PaletteBackground );
	setFrameStyle( QFrame::Box | QFrame::Raised );
	setResizePolicy( QScrollView::Manual );
	setVScrollBarMode( QScrollView::Auto );
	setHScrollBarMode( QScrollView::Auto );
	setTreeStepSize( 20 );
	setMultiSelection( FALSE );
	setAllColumnsShowFocus( TRUE );
	setItemMargin( 1 );
	setRootIsDecorated( FALSE );
	addColumn( "Name", -1 );
	setColumnWidthMode( 0, QListView::Maximum );
	setColumnAlignment( 0, 1 );
	addColumn( "Description", -1 );
	setColumnWidthMode( 1, QListView::Maximum );
	setColumnAlignment( 1, 1 );
	addColumn( "Driver", -1 );
	setColumnWidthMode( 2, QListView::Maximum );
	setColumnAlignment( 2, 1 );

}


CDSNList::~CDSNList()
{
}

void CDSNList::Load( int nSource )
{
	QListViewItem	*pListViewItem;
	QString			qsError;
	DWORD			nErrorCode;
	char   			szErrorMsg[101];
	
	char			szINI[FILENAME_MAX+1];
	char			szSectionNames[4096];
	char			szSectionName[INI_MAX_OBJECT_NAME+1];
	char			szDriver[INI_MAX_PROPERTY_VALUE+1];
	char			szDescription[INI_MAX_PROPERTY_VALUE+1];
	int				nElement;
	
	clear();
	this->nSource = nSource;

	// GET SECTION NAMES (Data Sources)
	strcpy( szINI, "odbc.ini" );
	memset( szSectionNames, 0, sizeof(szSectionNames) );
	SQLSetConfigMode( nSource );
	if ( SQLGetPrivateProfileString( NULL, NULL, NULL, szSectionNames, 4090, "odbc.ini" ) >= 0 )
	{
		for ( nElement = 0; iniElement( szSectionNames, '\0', '\0', nElement, szSectionName, INI_MAX_OBJECT_NAME ) == INI_SUCCESS ; nElement++ )
		{
			// GET DRIVER AND DESCRIPTION
			szDriver[0]		= '\0';
			szDescription[0]	= '\0';
#ifdef PLATFORM64
			SQLGetPrivateProfileString( szSectionName, "Driver64", "", szDriver, INI_MAX_PROPERTY_VALUE, "odbc.ini" );
			if ( szDriver[ 0 ] == '\0' )
			{
				SQLGetPrivateProfileString( szSectionName, "Driver", "", szDriver, INI_MAX_PROPERTY_VALUE, "odbc.ini" );
			}
#else
			SQLGetPrivateProfileString( szSectionName, "Driver", "", szDriver, INI_MAX_PROPERTY_VALUE, "odbc.ini" );
#endif
			SQLGetPrivateProfileString( szSectionName, "Description", "", szDescription, INI_MAX_PROPERTY_VALUE, "odbc.ini" );
			pListViewItem = new QListViewItem( this, szSectionName, szDescription, szDriver );
		}
		SQLSetConfigMode( ODBC_BOTH_DSN );
	}
	else
	{
		SQLSetConfigMode( ODBC_BOTH_DSN );
		qsError.sprintf( "Could not load %s", szINI );

        QMessageBox::information( this, "ODBC Config",  qsError );

        while ( SQLInstallerError( 1, &nErrorCode, szErrorMsg, 100, NULL ) == SQL_SUCCESS )
            QMessageBox::information( this, "ODBC Config",  szErrorMsg );

	}
}

void CDSNList::Add()
{
	// odbc.ini INFO
	QString				qsDataSourceName		= "";
	QString				qsDataSourceDescription	= "";
	QString				qsDataSourceDriver		= "";
	// odbcinst.ini INFO
	QString				qsDriverName			= "";
	QString				qsDriverDescription		= "";
	QString				qsDriverFile			= "";
	QString				qsSetupFile				= "";
	QString				qsError					= "";
	DWORD				nErrorCode;
	char				szErrorMsg[101];

	CDriverPrompt	 *pDriverPrompt;
	CPropertiesFrame *pProperties;
	HODBCINSTPROPERTY	 hFirstProperty	= NULL;
	HODBCINSTPROPERTY	 hCurProperty	= NULL;
	char				 szINI[FILENAME_MAX+1];

	pDriverPrompt = new CDriverPrompt( this );
	if ( pDriverPrompt->exec() )
	{
		qsDriverName		= pDriverPrompt->qsDriverName;
		qsDriverDescription	= pDriverPrompt->qsDescription;
		qsDriverFile		= pDriverPrompt->qsDriver;
		qsSetupFile			= pDriverPrompt->qsSetup;
		qsDataSourceDriver	= qsDriverName;
		delete pDriverPrompt;

        //
        // can we call SQLConfigDataSource ?
        //

        {
            int mode;

            if( nSource == ODBC_USER_DSN )
            {
                sprintf( szINI, "~/.odbc.ini" );
                mode = ODBC_ADD_DSN;
            }
            else
            {
                sprintf( szINI, "%s/odbc.ini", odbcinst_system_file_path());
                mode = ODBC_ADD_SYS_DSN;
            }
            if ( SQLConfigDataSource(( HWND ) 1, mode,
                (char*)qsDataSourceDriver.data(), "" ))
                                                                                            {
                Load( nSource );
                return;
            }
        }

		// GET PROPERTY LIST FROM DRIVER
		if ( ODBCINSTConstructProperties( (char*)(qsDataSourceDriver.latin1()), &hFirstProperty ) != ODBCINST_SUCCESS )
		{
			qsError.sprintf( "Could not construct a property list for (%s)", qsDataSourceDriver.data() );
			QMessageBox::information( this, "ODBC Config",  qsError );
			return;
		}

		// ALLOW USER TO EDIT
		pProperties = new CPropertiesFrame( this, "Properties", hFirstProperty );
        pProperties->setCaption( "Data Source Properties (new)" );
		if ( pProperties->exec() )
		{
			/* DELETE ENTIRE SECTION IF IT EXISTS (no entry given) */
			SQLSetConfigMode( nSource );
			if ( SQLWritePrivateProfileString( hFirstProperty->szValue, NULL, NULL, "odbc.ini" ) == FALSE )
			{
				SQLSetConfigMode( ODBC_BOTH_DSN );
				delete pProperties;
				ODBCINSTDestructProperties( &hFirstProperty );

				qsError.sprintf( "Could not write to (%s)", szINI );
				QMessageBox::information( this, "ODBC Config",  qsError );
				while ( SQLInstallerError( 1, &nErrorCode, szErrorMsg, 100, NULL ) == SQL_SUCCESS )
					QMessageBox::information( this, "ODBC Config",  szErrorMsg );

				return;
			}
			qsDataSourceName = hFirstProperty->szValue;
			/* ADD ENTRIES; SECTION CREATED ON FIRST CALL */
            QString stringName;
			for ( hCurProperty = hFirstProperty->pNext; hCurProperty != NULL; hCurProperty = hCurProperty->pNext )
			{
                stringName = hCurProperty->szName;
				if ( stringName.upper() == "DESCRIPTION" )
					qsDataSourceDescription = hCurProperty->szValue;

				SQLWritePrivateProfileString( hFirstProperty->szValue, hCurProperty->szName, hCurProperty->szValue, "odbc.ini" );
			}
			SQLSetConfigMode( ODBC_BOTH_DSN );
		}
		delete pProperties;
		ODBCINSTDestructProperties( &hFirstProperty );
	}
	else
		delete pDriverPrompt;

	// RELOAD (slow but safe)
	Load( nSource );
}

void CDSNList::Edit()
{
	// odbc.ini INFO
	QString				qsDataSourceName		= "";
	QString				qsDataSourceDescription		= "";
	QString				qsDataSourceDriver		= "";
	// odbcinst.ini INFO
	QString				qsDriverFile			= "";
	QString				qsSetupFile				= "";
	QString				qsError					= "";

	CPropertiesFrame		*pProperties;
	HODBCINSTPROPERTY	hFirstProperty	= NULL;
	HODBCINSTPROPERTY	hCurProperty	= NULL;
	QListViewItem		*pListViewItem;

	char				szEntryNames[4096];
	char				szProperty[INI_MAX_PROPERTY_NAME+1];
	char				szValue[INI_MAX_PROPERTY_VALUE+1];
	
	DWORD				nErrorCode;
	char				szErrorMsg[101];
	char				szINI[FILENAME_MAX+1];
	int					nElement;	

	// HAS THE USER SELECTED SOMETHING
    pListViewItem = currentItem();
    if ( pListViewItem )
    {
        qsDataSourceName		= pListViewItem->text( 0 );
        qsDataSourceDescription	= pListViewItem->text( 1 );
        qsDataSourceDriver		= pListViewItem->text( 2 );
    }
    else
    {
        QMessageBox::information( this, "ODBC Config",  "Please select a Data Source from the list first" );
        return;
    }

    //
    // can we call SQLConfigDataSource ?
    //

    {
        char attr[ 128 ];
        int mode;

        sprintf( attr, "DSN=%s;", ( const char * ) qsDataSourceName );

        if( nSource == ODBC_USER_DSN )
        {
            sprintf( szINI, "~/.odbc.ini" );
            mode = ODBC_CONFIG_DSN;
        }
        else
        {
            sprintf( szINI, "%s/odbc.ini", odbcinst_system_file_path());
            mode = ODBC_CONFIG_SYS_DSN;
        }

        if ( SQLConfigDataSource(( HWND ) 1, mode,
            (char*)qsDataSourceDriver.data(), attr ))
        {
			SQLSetConfigMode( ODBC_BOTH_DSN );
            Load( nSource );
            return;
        }
        SQLSetConfigMode( ODBC_BOTH_DSN );
    }

	// GET PROPERTY LIST FROM DRIVER
	if ( ODBCINSTConstructProperties( (char*)qsDataSourceDriver.data(), &hFirstProperty ) != ODBCINST_SUCCESS )
	{
		qsError.sprintf( "Could not construct a property list for (%s)", qsDataSourceDriver.data() );
		QMessageBox::information( this, "ODBC Config",  qsError );
		while ( SQLInstallerError( 1, &nErrorCode, szErrorMsg, 100, NULL ) == SQL_SUCCESS )
			QMessageBox::information( this, "ODBC Config",  szErrorMsg );

		return;
	}

	// COPY EXISTING VALUES INTO PROPERTIES LIST
	SQLSetConfigMode( nSource );
	ODBCINSTSetProperty( hFirstProperty, "Name", (char*)(qsDataSourceName.latin1()) );
    memset( szEntryNames, 0, sizeof( szEntryNames ));
	SQLGetPrivateProfileString((char*)qsDataSourceName.data(), NULL, NULL, szEntryNames, 4090, "odbc.ini" ); // GET ALL ENTRY NAMES FOR THE SELCTED DATA SOURCE
	for ( nElement = 0; iniElement( szEntryNames, '\0', '\0', nElement, szProperty, 1000 ) == INI_SUCCESS ; nElement++ )
	{
		SQLGetPrivateProfileString((char*) qsDataSourceName.data(), szProperty, "", szValue, INI_MAX_PROPERTY_VALUE, szINI ); // GET VALUE FOR EACH ENTRY

		if ( ODBCINSTSetProperty( hFirstProperty, szProperty, szValue ) == ODBCINST_ERROR )
        {
		    ODBCINSTAddProperty( hFirstProperty, szProperty, szValue );
        }
	}
	SQLSetConfigMode( ODBC_BOTH_DSN );

	// ALLOW USER TO EDIT
	pProperties = new CPropertiesFrame( this, "Properties", hFirstProperty );
	pProperties->setCaption( "Data Source Properties (edit)" );
	if ( pProperties->exec() )
	{
		SQLSetConfigMode( nSource );
		/* DELETE ENTIRE SECTION IF IT EXISTS (given NULL entry) */
		if ( SQLWritePrivateProfileString((char*) qsDataSourceName.ascii(), NULL, NULL, "odbc.ini" ) == FALSE )
		{
			SQLSetConfigMode( ODBC_BOTH_DSN );
			delete pProperties;
			ODBCINSTDestructProperties( &hFirstProperty );

			qsError.sprintf( "Could not write to (%s)", szINI );
			QMessageBox::information( this, "ODBC Config",  qsError );
			while ( SQLInstallerError( 1, &nErrorCode, szErrorMsg, 100, NULL ) == SQL_SUCCESS )
				QMessageBox::information( this, "ODBC Config",  szErrorMsg );

			return;
		}
		qsDataSourceName = hFirstProperty->szValue;
		/* ADD ENTRIES; SECTION CREATED ON FIRST CALL */
        QString stringName;
		for ( hCurProperty = hFirstProperty->pNext; hCurProperty != NULL; hCurProperty = hCurProperty->pNext )
		{
            stringName = hCurProperty->szName;
			if ( stringName.upper() == "DESCRIPTION" )
				qsDataSourceDescription = hCurProperty->szValue;

			SQLWritePrivateProfileString( hFirstProperty->szValue, hCurProperty->szName, hCurProperty->szValue, szINI );
		}
		SQLSetConfigMode( ODBC_BOTH_DSN );
	}
	delete pProperties;
	ODBCINSTDestructProperties( &hFirstProperty );

	// RELOAD (slow but safe)
	Load( nSource );
}

void CDSNList::Delete()
{
	QListViewItem		*pListViewItem;
	char 				szINI[FILENAME_MAX+1];
	char 				*pDataSourceName;
	QString				qsError;
	DWORD				nErrorCode;
	char				szErrorMsg[FILENAME_MAX+1];

	// GET SELECT DATA SOURCE NAME
    pListViewItem = currentItem();
	if ( pListViewItem )
	{
		pDataSourceName = (char *)pListViewItem->text( 0 ).ascii();
	}
	else
	{
		QMessageBox::information( this, "ODBC Config",  "Please select a Data Source from the list first" );
		return;
	}

	// DELETE ENTIRE SECTION IF IT EXISTS (given NULL entry)
	SQLSetConfigMode( nSource );
	if ( SQLWritePrivateProfileString( pDataSourceName, NULL, NULL, szINI ) == FALSE )
	{
		qsError.sprintf( "Could not write property list for (%s)", pDataSourceName );
		QMessageBox::information( this, "ODBC Config",  qsError );
		while ( SQLInstallerError( 1, &nErrorCode, szErrorMsg, FILENAME_MAX, NULL ) == SQL_SUCCESS )
			QMessageBox::information( this, "ODBC Config",  szErrorMsg );
	}
    else
        QMessageBox::information( this, "ODBC Config", "Done!" );

	SQLSetConfigMode( ODBC_BOTH_DSN );
	
	// RELOAD (slow but safe)
	Load( nSource );
}

void CDSNList::DoubleClick( QListViewItem * itm )
{
    Edit();
}
