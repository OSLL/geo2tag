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
#ifndef classDataSources_included
#define classDataSources_included

#include "classNode.h"
#include "classCanvas.h"
#include "classDataSource.h"
#include "classODBC.h"
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include <qlistview.h>
#include <sqlext.h>

class classDataSources: public classNode
{
public:
    classDataSources( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, classODBC::DSType dataSourceType );
   ~classDataSources() {}

    void setOpen( bool bOpen );
    void selectionChanged( QListViewItem * );

private:
  QList<classDataSource> listDataSources;
  classODBC::DSType      dataSourceType;

  void Init();
};
#endif

