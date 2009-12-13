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
#ifndef classDrivers_included
#define classDrivers_included

#include "classCanvas.h"
#include "classDriver.h"
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include <qlistview.h>
#include <sqlext.h>

class classDrivers: public classNode
{
public:
    classDrivers( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas );
   ~classDrivers() {}

    void setOpen( bool );

private:
    QList<classDriver> listDrivers;

};
#endif

