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
#ifndef CDSNList_included
#define CDSNList_included

#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <ini.h>
#include <odbcinstext.h>

#include <qapplication.h>
#include <qmessagebox.h>
#include <qwidget.h>
#include <qlistview.h>
#include <qstring.h>

#include "CDriverPrompt.h"
#include "CPropertiesFrame.h"
#include "CDLL.h"

#define ODBC_HELP_DSN_NAME "*Unique* DSN name. This is the name you use when using ODBC with applications such as StarOffice. Try to keep unusual characters and spaces out of the name."
#define ODBC_HELP_DSN_DESC "DSN description. A long, perhaps more meaningfull name."
#define ODBC_HELP_DSN_UNKNOWN "No help for this DSN property. Please check with the vendor of the driver... perhaps their web site"

class CDSNList : public QListView
{
    Q_OBJECT

public:
    CDSNList( QWidget* parent = NULL, const char* name = NULL );
    ~CDSNList();
	
	void Load( int nSource );

public slots:
	void Add();
	void Edit();
	void Delete();
    void DoubleClick( QListViewItem * itm );

private:
	int nSource;
};
#endif

