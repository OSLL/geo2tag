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

#include <odbcinst.h>
#include <qapplication.h>
#include <qmessagebox.h>

#if defined ( ODBCINSTQ_STATIC )
#include "../odbcinstQ/CODBCConfig.h"
#endif

#ifdef Q_WS_MACX
#include "../odbcinstQ/CSplashDialog.h"
#endif

int main( int argc, char **argv )
{
    QApplication::setDesktopSettingsAware( true );             // try to use desktop colors
    QApplication oApplication( argc, argv );

#ifdef Q_WS_MACX
    QSettings settings;
    if ( !settings.readEntry( "/unixODBC/NoSplash" ) )
    {
        CSplashDialog splashdialog( 0 );
        splashdialog.exec();
    }
#endif

#if defined ( ODBCINSTQ_STATIC )
#if QT_VERSION<300
    CODBCConfig odbcconfig( 0, "ODBCConfig", Qt::WType_Modal );
#else
//    CODBCConfig odbcconfig( 0, "ODBCConfig", Qt::WType_Dialog | Qt::WShowModal );
    CODBCConfig odbcconfig( 0, "ODBCConfig" );
#endif
    oApplication.connect( &oApplication, SIGNAL(lastWindowClosed()), &oApplication, SLOT(quit()) );
    return odbcconfig.exec();
#else
    int nReturn;
    ODBCINSTWND odbcinstwnd;
    
    odbcinstwnd.szGUI[0]    = 'Q';
    odbcinstwnd.szGUI[1]    = 't';
    odbcinstwnd.szGUI[2]    = '\0';
    odbcinstwnd.hWnd        = qApp->desktop();

    nReturn = SQLManageDataSources( (HWND)(&odbcinstwnd) );
    if ( !nReturn )
    {
        QMessageBox::warning( 0, "ODBCConfig",  "Failed to execute SQLManageDataSources()." );
        QMessageBox::information( 0, "ODBCConfig",  "The most likely reason for this is that the Qt GUI plugin could not be\nfound or could not be loaded.\nEnsure that libodbcinstQ.* files are in the library search path.\nThe path can be altered by setting the LTDL_LIBRARY_PATH environment variable." );
    }

    return nReturn;
#endif
}

