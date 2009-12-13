#ifndef CSPLASHDIALOG_H
#define CSPLASHDIALOG_H

#include <qobject.h>
#include <qwidget.h>
#include <qdialog.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#if (QT_VERSION>=300)
#include <qsettings.h>
#endif

class CSplashDialog : public QDialog
{
public:
    CSplashDialog( QWidget *pwidgetParent );
    virtual ~CSplashDialog();

protected:
   QCheckBox *pcheckbox;
};

#endif

