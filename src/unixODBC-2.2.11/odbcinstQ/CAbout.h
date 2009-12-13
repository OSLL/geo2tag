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
#ifndef CAbout_included
#define CAbout_included

#include <qwidget.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qlabel.h>
#if (QT_VERSION>=300)
#include <qpushbutton.h>
#else
#include <qpushbt.h>
#endif
#include <qframe.h>
#include <qmovie.h>

#include "CCredits.h"


class CAboutDiagram : public QWidget
{
    Q_OBJECT

public:

    CAboutDiagram( QWidget* pwidgetParent = NULL, const char* pszName = NULL );
    virtual ~CAboutDiagram();

protected slots:
    void pbODBCConfig_Clicked();
    void pbODBC_Clicked();
    void pbDatabase_Clicked();
    void pbDriverManager_Clicked();
    void pbDriver_Clicked();
    void pbODBCDrivers_Clicked();
    void pbApplication_Clicked();
};

class CAbout : public QWidget
{
    Q_OBJECT

public:

    CAbout( QWidget* pwidgetPrent = NULL, const char* pszName = NULL );
    virtual ~CAbout();

protected slots:
    void pbCredits_Clicked();
};

#endif 
