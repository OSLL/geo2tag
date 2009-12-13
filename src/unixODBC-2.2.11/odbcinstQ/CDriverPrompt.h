/**************************************************
 * 
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign. 
 * Released under GPL 31.JAN.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/
#ifndef CDriverPrompt_included
#define CDriverPrompt_included

#include <qdialog.h>
#include <qframe.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qlabel.h>
#if (QT_VERSION>=300)
#include <qpushbutton.h>
#else
#include <qpushbt.h>
#endif

#include "CDrivers.h"

class CDriverPrompt : public QDialog
{
    Q_OBJECT

public:
    CDriverPrompt( QWidget* parent = NULL, const char* name = NULL );
    ~CDriverPrompt();

	QString	qsDriverName;
	QString	qsDescription;
	QString	qsDriver;
	QString	qsSetup;

protected slots:

    void pbCancel_Clicked();
    void pbOk_Clicked();

protected:
    CDrivers* pDrivers;
};
#endif 
