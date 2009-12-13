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
#ifndef classTables_included
#define classTables_included

#include <qlistview.h>
#include <qstring.h>
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include "classNode.h"
#include "classCanvas.h"
#include "classTable.h"
#include <sqlext.h>

class classTables: public classNode
{
public:
   classTables( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHDBC hDbc, const char * szLibrary = 0 );
  ~classTables() {}

   void setOpen( bool bOpen );
   void selectionChanged( QListViewItem * );

private:
  QList<classTable> listTables;
  SQLHDBC           hDbc;
  QString           qsLibrary;

  void LoadTables();

};
#endif

