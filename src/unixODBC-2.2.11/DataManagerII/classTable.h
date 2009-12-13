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
#ifndef classTable_included
#define classTable_included

#include "classCanvas.h"
#include "classColumn.h"
#include "classPrimaryKeys.h"
#include "classSpecialColumns.h"
#include "classIndexs.h"
#include "classBrowseFrame.h"
#include <qstring.h>
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include <qlistview.h>
#include <sqlext.h>

class classTable: public classNode
{
public:
   classTable( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHDBC hDbc, const char *pszName, const char *pszType = 0, const char *pszDescription = 0, const char *pszLibraryName = 0 );
  ~classTable();

   void setOpen( bool bOpen );
   void selectionChanged( QListViewItem * );

private:
  QList<classColumn>  listColumns;
  classPrimaryKeys    *pPrimaryKeys;
  classSpecialColumns *pSpecialColumns;
  classIndexs         *pIndexs;
  classBrowseFrame    *pBrowse;
  SQLHDBC             hDbc;
  QString             qsLibrary;
  QString             qsTable;

  void Fini();
  void LoadColumns();
};
#endif

