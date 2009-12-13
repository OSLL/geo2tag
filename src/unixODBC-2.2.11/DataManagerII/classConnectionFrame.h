/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 30.NOV.00
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey   - pharvey@codebydesign.com
 **************************************************/
#ifndef classConnectionFrame_included
#define classConnectionFrame_included

#include "classISQL.h"
#include <qwidget.h>
#include <qmainwindow.h>
#include <qpopupmenu.h>
#include <sqlext.h>

class classConnectionFrame : public QMainWindow
{
    Q_OBJECT

public:

    classConnectionFrame( SQLHDBC hDbc, QString qsDataSource, QWidget* parent = 0, const char* name = 0 );
   ~classConnectionFrame() {}

    void resizeEvent( QResizeEvent * );

public slots:
    void Exec();
    void setViewHTMLSource();
    void setViewText();
    void setViewTextDelimited();

protected:
    QPopupMenu *    pView;
    classISQL  *    isql;
    classISQL::View view;
};
#endif

