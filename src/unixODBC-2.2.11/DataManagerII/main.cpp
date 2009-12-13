/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 18.FEB.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey         - pharvey@codebydesign.com
 **************************************************/

#include <qapplication.h>
#include <qdir.h>
#include "classDataManager.h"

int main( int argc, char **argv )
{
    QApplication        oApplication( argc, argv );                 // create application object
    classDataManager    oMain;

    QDir::home().mkdir( ".DataManager" );

    oApplication.connect( &oApplication, SIGNAL(lastWindowClosed()), &oApplication, SLOT(quit()) );
    oMain.show();                                                   // show widget

    return oApplication.exec();                                     // run event loop
}

