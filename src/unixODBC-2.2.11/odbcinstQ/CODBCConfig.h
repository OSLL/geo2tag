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
#ifndef CODBCConfig_included
#define CODBCConfig_included

#include <qwidget.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qdialog.h>
#include <qtabbar.h>
#include <qframe.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qtabdialog.h>
#if QT_VERSION>=300
#include <qsettings.h>
#endif

#include "CUserDSN.h"
#include "CSystemDSN.h"
#include "CFileDSN.h"
#include "CDrivers.h"
#include "CAbout.h"
#include "CTracing.h"
#include "CStats.h"

class CODBCConfig : public QTabDialog
{
    Q_OBJECT

public:
    CODBCConfig( QWidget* parent = 0, const char* name = 0, WFlags nFlags = 0 );
    virtual ~CODBCConfig();

protected:
    CUserDSN    *pUserDSN;
    CSystemDSN  *pSystemDSN;
    CFileDSN    *pFileDSN;
    CDrivers    *pDrivers;
    CStats      *pStats;
    CTracing    *pTracing;
    CAbout      *pAbout;

    virtual void LoadState();
    virtual void SaveState();
};
#endif 


