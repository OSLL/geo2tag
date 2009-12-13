/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 30.NOV.00
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey         - pharvey@codebydesign.com
 **************************************************/
#ifndef classBrowseFrame_included
#define classBrowseFrame_included

#include "classBrowse.h"
#include <qwidget.h>
#include <qmainwindow.h>
#include <qpopupmenu.h>
#include <qstring.h>
#include <sqlext.h>

class classBrowseFrame : public QMainWindow
{
    Q_OBJECT

public:

    classBrowseFrame( SQLHDBC hDbc, const QString &qsTable, const QString &qsLibrary, QWidget* parent = 0, const char* name = 0 );
    ~classBrowseFrame() {}

    void resizeEvent( QResizeEvent * );

public slots:
    void autoRefresh();

protected:
    classBrowse *browse;
    QPopupMenu  *pResults;
    int         nAutoRefresh;
};
#endif

