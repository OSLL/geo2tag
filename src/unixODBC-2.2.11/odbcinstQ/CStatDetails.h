#ifndef CLASSSTATDETAILS_H
#define CLASSSTATDETAILS_H

#include <qwidget.h>
#include <qframe.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qgrid.h>
#include <qvbox.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtimer.h>
#include <qstring.h>
#include <qtable.h>
#include <uodbc_stats.h>

#define MAXPROCESSES 10
#define MAXHANDLES 4

class CStatDetails : public QWidget
{

	Q_OBJECT

public:
	CStatDetails( QWidget* parent = NULL, const char* name = NULL );
	virtual ~CStatDetails();

    QTimer *pTimer;

protected:
    void clearRow( int nRow );

protected slots:
    void showStats();

private:
    QTable *    pTable;
    void *      hStats;
    uodbc_stats_retentry aPIDs[MAXPROCESSES];
    uodbc_stats_retentry aHandles[MAXHANDLES];
};

#endif


