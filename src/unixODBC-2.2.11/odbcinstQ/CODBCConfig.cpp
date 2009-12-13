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
#include "CODBCConfig.h"

#include "ODBC.xpm"

CODBCConfig::CODBCConfig( QWidget* parent, const char* name, WFlags nFlags )
	: QTabDialog( parent, name, nFlags )
{
    setIcon( QPixmap( xpmODBC ) );
    setCaption( "ODBC Data Source Administrator" );
    setSizeGripEnabled( true );

    pUserDSN       = new CUserDSN( this );
    addTab( pUserDSN, "&User DSN" );

    pSystemDSN     = new CSystemDSN( this );
    addTab( pSystemDSN, "&System DSN" );

    pFileDSN       = new CFileDSN( this, NULL );
    addTab( pFileDSN, "&File DSN" );

    pDrivers       = new CDrivers( this );
    addTab( pDrivers, "&Drivers" );

    pStats         = new CStats( this );
    addTab( pStats, "St&ats" );

    pTracing       = new CTracing( this );
    addTab( pTracing, "Ad&vanced" );

    pAbout         = new CAbout( this );
    addTab( pAbout, "&About" );

	resize( 500,330 );
	setMinimumSize( 0, 0 );
	setMaximumSize( 32767, 32767 );

    LoadState();
}

CODBCConfig::~CODBCConfig()
{
    SaveState();
}

void CODBCConfig::LoadState()
{
#if QT_VERSION>=300
    QSettings settings;

    // main window
    {
#ifdef Q_WS_X11
        int nX = settings.readNumEntry( "/unixODBC/ODBCConfig/x", geometry().x() );
        int nY = settings.readNumEntry( "/unixODBC/ODBCConfig/y", geometry().y() );
        int nW = settings.readNumEntry( "/unixODBC/ODBCConfig/w", geometry().width() );
        int nH = settings.readNumEntry( "/unixODBC/ODBCConfig/h", geometry().height() );
        setGeometry( nX, nY, nW, nH );
#else
        int nW = settings.readNumEntry( "/unixODBC/ODBCConfig/w", geometry().width() );
        int nH = settings.readNumEntry( "/unixODBC/ODBCConfig/h", geometry().height() );
        resize( nW, nH );
#endif
    }
#endif
}

void CODBCConfig::SaveState()
{
#if QT_VERSION>=300
    QSettings settings;

    // main window
    settings.writeEntry( "/unixODBC/ODBCConfig/x", x() );
    settings.writeEntry( "/unixODBC/ODBCConfig/y", y() );
    settings.writeEntry( "/unixODBC/ODBCConfig/w", width() );
    settings.writeEntry( "/unixODBC/ODBCConfig/h", height() );
#endif
}

