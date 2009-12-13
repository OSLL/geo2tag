#ifndef CLASSTRACING_H
#define CLASSTRACING_H

#include <odbcinstext.h>

#include <qwidget.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qgrid.h>
#include <qvbox.h>
#include <qlayout.h>
#include <qpushbutton.h>

#include "CFileSelector.h"

class CTracing : public QWidget
{
	Q_OBJECT
public:
	CTracing( QWidget* parent = NULL, const char* name = NULL );
	virtual ~CTracing();

public slots:
    void setDefault();
	void apply();

protected:
	QCheckBox *         pTracing;
	QCheckBox *         pForce;
	CFileSelector *     pTraceFile;
	QCheckBox *         pPooling;
};

#endif


