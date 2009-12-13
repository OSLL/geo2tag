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
#ifndef classBrowseFrame_included
#define classBrowseFrame_included

#include <qwidget.h>
#include <qlayout.h>
#include <qtable.h>
#include <qmessagebox.h>
#include <qwmatrix.h>

#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qmenubar.h>
#include <qwhatsthis.h>

#include <sqlext.h>

#include "classBrowse.h"

class classBrowseFrame : public QMainWindow
{
    Q_OBJECT

public:

    classBrowseFrame( SQLHDBC hDbc, QString qsTable, QWidget* parent = 0, const char* name = 0 );
    ~classBrowseFrame();

    void resizeEvent( QResizeEvent * );

public slots:
    void Clear();         // CLEAR QUERY AND DATA
    void Save();        // DATA
    void SaveAs();      // DATA
    void Exec();        // RUN QUERY

protected:
    QMenuBar        *menubarMain;
    QToolBar        *toolbarMain;
    classBrowse     *browse;
	SQLHDBC 		hDbc;
    QString     	qsTable;

};

#endif


