/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 18.FEB.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey   - pharvey@codebydesign.com
 **************************************************/
#ifndef classLogin_included
#define classLogin_included

class ConnectionScoper ;
#include <qdialog.h>
#include <qwidget.h>
#include <qlineedit.h>
#include <qstring.h>
#include <sqlext.h>

class classLogin: public QDialog
{
  Q_OBJECT

public:
  classLogin( QWidget *pParent, ConnectionScoper &dbc, const QString &dataSource );
 ~classLogin() {}

private slots:
  void pbOk_Clicked();
  void pbCancel_Clicked();

private:
  ConnectionScoper &dbc;
  QString   qsDataSourceName;
  QLineEdit *txtUID;
  QLineEdit *txtPWD;
};
#endif

