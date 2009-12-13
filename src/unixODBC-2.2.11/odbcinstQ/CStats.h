#ifndef CLASSSTATS_H
#define CLASSSTATS_H

#include <qwidget.h>
#include <qgrid.h>
#include <qvbox.h>
#include <qlayout.h>
#include <qtabbar.h>
#include <qpixmap.h>

#include "CStatSummary.h"
#include "CStatDetails.h"

class CStats : public QWidget
{

	Q_OBJECT

public:
	CStats( QWidget* parent = NULL, const char* name = NULL );
	virtual ~CStats();

public slots:

protected:

protected slots:

private:
    CStatSummary * pSummary;
    CStatDetails * pDetails;

};

#endif


