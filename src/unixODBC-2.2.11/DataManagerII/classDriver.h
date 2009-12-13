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
#ifndef classDriver_included
#define classDriver_included

#include "classCanvas.h"
#include "classNode.h"
#include <qstring.h>
#include <qlistview.h>
#include <sqlext.h>

class classDriver: public classNode
{
public:
    classDriver( QListViewItem  *pParent, QListViewItem *pAfter, classCanvas *pCanvas, const char *pszDriverName = 0 );
   ~classDriver() {}

private:
    QString qsDriverName;

};
#endif

