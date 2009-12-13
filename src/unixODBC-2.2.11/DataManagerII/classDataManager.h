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
#ifndef classDataManager_included
#define classDataManager_included

#include "classCanvas.h"
#include "classODBC.h"
#include <qwidget.h>
#include <qsplitter.h>
#include <qlistview.h>

class classDataManager : public QWidget
{
    Q_OBJECT

public:
    classDataManager( QWidget *pParent = 0, const char *pszName = 0 );
    ~classDataManager();

    void resizeEvent( QResizeEvent * );

public slots:
    void ItemMenu(QListViewItem*,const QPoint&,int);
    void ItemChanged(QListViewItem*);

protected slots:
    void Exit();
    void About();

protected:
    QSplitter   *splSplitter;
    QListView   *lvwBrowser;
    classCanvas *pCanvas;
    classODBC   *pODBC;

private:
    void LoadState();
    void SaveState();
};
#endif

