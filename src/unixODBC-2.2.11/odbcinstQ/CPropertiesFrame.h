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
#ifndef CPropertiesFrame_included
#define CPropertiesFrame_included

#include <qwidget.h>
#include <qdialog.h>
#include <qlayout.h>
#if (QT_VERSION>=300)
#include <qsettings.h>
#endif

#include <odbcinstext.h>

#include "CProperties.h"

class CPropertiesFrame : public QDialog
{
    Q_OBJECT

public:
    CPropertiesFrame( QWidget* parent = NULL, const char* name = NULL, HODBCINSTPROPERTY hTheFirstProperty = NULL );
    ~CPropertiesFrame();

protected:
    virtual void doLoadState();
    virtual void doSaveState();

protected slots:
    void doOk();
    void doCancel();

private:
    CProperties *pProperties;
};

#endif
