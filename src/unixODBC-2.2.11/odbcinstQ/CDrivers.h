/**************************************************
 * CDrivers
 *
 * Drivers can be accessed via at least three methods. I
 * show all three in use in the constructer but I
 * recommend using SQLDrivers in the Driver Manager as
 * the best method.
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 31.JAN.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/
#ifndef CDrivers_included
#define CDrivers_included

#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

#include <ini.h>
#include <odbcinstext.h>

#include <qmessagebox.h>
#include <qwidget.h>
#include <qlistview.h>
#if (QT_VERSION>=300)
#include <qpushbutton.h>
#else
#include <qpushbt.h>
#endif
#include <qpixmap.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qframe.h>
#include <qlistview.h>

#include "CPropertiesFrame.h"

#define ODBC_HELP_DRIVER_NAME "*Unique* driver name."
#define ODBC_HELP_DRIVER_DESC "Driver description."
#define ODBC_HELP_DRIVER_DRIVER "The driver. Its a share library and the filename will probably have odbc in it and it will probably end with *.so."
#define ODBC_HELP_DRIVER_DRIVER64 "The driver for 64 bit environments. Its a share library and the filename will probably have odbc in it and it will probably end with *.so."
#define ODBC_HELP_DRIVER_SETUP "The setup routines. Its a share library used to provide this program with DSN properties which can be presented to the user to edit. This library filename usually ends with a *S.so."
#define ODBC_HELP_DRIVER_SETUP64 "The setup routines for 64 bit environments. Its a share library used to provide this program with DSN properties which can be presented to the user to edit. This library filename usually ends with a *S.so."
#define ODBC_HELP_DRIVER_USAGECOUNT "The number of installs that use this driver. This driver entry should be removed when < 1."
#define ODBC_HELP_DRIVER_CPTIMEOUT "Number of seconds before a connection timesout when in a Connection Pool. Leave this value blank to disable Connection Pooling."
#define ODBC_HELP_DRIVER_CPREUSE "The maximum number of times a connection can be reused in a Connection Pool. Set to a lower number when dealing with drivers which have stability issues or memory leaks."
#define ODBC_HELP_DRIVER_UNKNOWN "No help for this driver specific property. Please check with the vendor of the driver... perhaps their web site"

class CDrivers : public QWidget
{
    Q_OBJECT

public:

    CDrivers( QWidget* parent = NULL, const char* name = NULL );
    ~CDrivers();

	QListView *getListView() { return lvwDrivers; };

public slots:
	void Add();
	void Edit();
	void Delete();

protected:
    QPushButton* pbAdd;
    QPushButton* pbRemove;
    QPushButton* pbConfigure;
    QListView* lvwDrivers;

private:
	HINI	hIni;
	char 	szINI[ODBC_FILENAME_MAX+1];

	void	Load();
	void	FreeProperties( HODBCINSTPROPERTY *hFirstProperty );

};
#endif

