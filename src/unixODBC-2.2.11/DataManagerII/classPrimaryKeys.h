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
#ifndef classPrimaryKeys_included
#define classPrimaryKeys_included

#include "classCanvas.h"
#include "classColumn.h"
#include <qstring.h>
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include <qlistview.h>
#include <sqlext.h>

class classPrimaryKeys: public classNode
{
public:
   classPrimaryKeys( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHDBC hDbc, const char *pszTable = 0, const char *pszLibrary = 0 );
  ~classPrimaryKeys() {}

   void setOpen( bool );

private:
  QList<classColumn>  listColumns;
  SQLHDBC             hDbc;
  QString             qsLibrary;
  QString             qsTable;

  void LoadColumns();

};
#endif

