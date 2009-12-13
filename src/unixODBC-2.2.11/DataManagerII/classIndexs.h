/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 18.FEB.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey   - pharvey@codebydesign.com
 **************************************************/
#ifndef classIndexs_included
#define classIndexs_included

#include "classCanvas.h"
#include "classIndex.h"
#include <qstring.h>
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include <qlistview.h>
#include <sqlext.h>

class classIndexs: public classNode
{
public:
   classIndexs( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHDBC hDbc, const char *pszTable = 0, const char *pszLibrary = 0 );
  ~classIndexs() {}

   void setOpen( bool );

private:
  QList<classColumn> listIndexs;
  SQLHDBC            hDbc;
  QString            qsLibrary;
  QString            qsTable;

  void LoadIndexs();

};
#endif

