/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 30.NOV.00
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/
#ifndef classConnectionFrame_included
#define classConnectionFrame_included

#include <qwidget.h>
#include <qlayout.h>
#include <qtable.h>
#include <qmessagebox.h>
#include <qwmatrix.h>

#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qwhatsthis.h>

#include <sqlext.h>

#include "classISQL.h"

class classConnectionFrame : public QMainWindow
{
    Q_OBJECT

public:

    classConnectionFrame( SQLHDBC hDbc, QString qsDataSource, QWidget* parent = 0, const char* name = 0 );
    ~classConnectionFrame();

    void resizeEvent( QResizeEvent * );

public slots:
    void New();         // CLEAR QUERY AND DATA
    void Open();
    void Save();        // DATA
    void SaveAs();      // DATA
    void Exec();        // RUN QUERY
    void setViewHTML();
    void setViewHTMLSource();
    void setViewGUI();
    void setViewText();
    void setViewTextDelimited();

protected:
    QMenuBar *      menubarMain;
    QToolBar *      toolbarMain;
    QStatusBar *    statusbarMain;
    classISQL *     isql;
    QPopupMenu *    pView;
    int             nViewHTML;
    int             nViewHTMLSource;
    int             nViewGUI;
    int             nViewText;
    int             nViewTextDelimited;
    int             nView;
};

#endif


