#ifndef CLASSSTATSUMMARY_H
#define CLASSSTATSUMMARY_H

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
#include <qslider.h>
#include <uodbc_stats.h>

class CStatSummary : public QWidget
{

	Q_OBJECT

public:
	CStatSummary( QWidget* parent = NULL, const char* name = NULL );
	virtual ~CStatSummary();

    QTimer *pTimer;

    QLabel *pEnv;
    QLabel *pCon;
    QLabel *pSta;
    QLabel *pDes;

    QSlider *pEnvSlider;
    QSlider *pConSlider;
    QSlider *pStaSlider;
    QSlider *pDesSlider;

protected:
    int nSliderMax;

protected slots:
    void showStats();

private:
    void *hStats;
    uodbc_stats_retentry aStats[4];
};

#endif


