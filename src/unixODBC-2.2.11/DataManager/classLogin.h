/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 18.FEB.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/
#ifndef classLogin_included
#define classLogin_included

#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

#include <qdialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qstring.h>

#include <sqlext.h>

class classLogin: public QDialog
{
	Q_OBJECT

public:
    classLogin( QWidget	*pParent, SQLHDBC hDbc, char *pszDataSource, int nDataSourceType = classLogin::User );
    ~classLogin();

	enum DataSourceTypes
	{
		User,
		System
	};

private slots:
	void pbOk_Clicked();
	void pbCancel_Clicked();

private:
	SQLHDBC		hDbc;
	int			nDataSourceType;
	QString		qsDataSourceName;
	QLineEdit	*txtUID;
	QLineEdit	*txtPWD;
};

#endif

