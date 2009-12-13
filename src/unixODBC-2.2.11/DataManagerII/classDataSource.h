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
#ifndef classDataSource_included
#define classDataSource_included

#include "classCanvas.h"
#include "classTables.h"
#include "classConnectionFrame.h"
#include "classODBC.h"
#include <qstring.h>
#include <qlistview.h>
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include <sqlext.h>

class classDataSource: public classNode
{
public:
    classDataSource( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, classODBC::DSType dataSourceType, const char *pszDataSourceName = 0 );
   ~classDataSource();

    void setOpen( bool bOpen );
    void selectionChanged ( QListViewItem * );
    void LoadLibraries();

private:
    QList<classTables>      listLibraries;
    classConnectionFrame *  pConnectionFrame;
    QString                 qsDataSourceName;
    EnvironmentScoper       env;
    ConnectionScoper        dbc;
};
#endif

