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
#ifndef CSystemDSN_included
#define CSystemDSN_included

#include <qwidget.h>
#include <qpixmap.h>
#include <qlayout.h>

#include "CDSNList.h"

class CSystemDSN : public QWidget
{
    Q_OBJECT

public:

    CSystemDSN( QWidget* parent = NULL, const char* name = NULL );
    ~CSystemDSN();

protected:
    QPushButton* pbAdd;
    QPushButton* pbRemove;
    QPushButton* pbConfigure;
    CDSNList* pDSNList;
};
#endif
