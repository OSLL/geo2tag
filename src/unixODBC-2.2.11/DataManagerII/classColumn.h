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
#ifndef classColumn_included
#define classColumn_included

#include "classNode.h"
#include "classCanvas.h"
#include <qlistview.h>
#include <sqlext.h>

class classColumn: public classNode
{
public:
    classColumn( QListViewItem  *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHDBC hDbc, const char *pszName = 0, const char *pszType = 0, const char *pszDesc = 0 );
   ~classColumn() {}
};
#endif

