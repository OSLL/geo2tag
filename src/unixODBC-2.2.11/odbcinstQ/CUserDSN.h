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
#ifndef CUserDSN_included
#define CUserDSN_included

#include <qwidget.h>
#if (QT_VERSION>=300)
#include <qpushbutton.h>
#else
#include <qpushbt.h>
#endif
#include <qpixmap.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qframe.h>

#include "CDSNList.h"

class CUserDSN : public QWidget
{
    Q_OBJECT

public:

    CUserDSN( QWidget* parent = NULL, const char* name = NULL );
    ~CUserDSN();

protected:
    QPushButton* pbAdd;
    QPushButton* pbRemove;
    QPushButton* pbConfigure;
    CDSNList* pDSNList;
};
#endif 
