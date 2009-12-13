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
#ifndef CProperties_included
#define CProperties_included

#include <qlayout.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qlistbox.h>
#include <qcombobox.h>
#include <qtoolbar.h>
#include <qtooltip.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>
#include <qmainwindow.h>

#include <odbcinstext.h>

#include "CFileSelector.h"

class CProperties : public QMainWindow
{
    Q_OBJECT

public:
    CProperties( QWidget* parent = NULL, const char* name = NULL, HODBCINSTPROPERTY hTheFirstProperty = NULL );
    ~CProperties();

protected:
    QToolBar        *toolbarMain;

//	void resizeEvent( QResizeEvent *p );

protected slots:
	void pbOk_Clicked();
	void pbCancel_Clicked();
    
signals:
    void Ok();
    void Cancel();

private:
    QWidget             *pMainWidget;
	int					nProperties;
	QBoxLayout 			*pTopLayout;
	QGridLayout			*pGridLayout;
	HODBCINSTPROPERTY	hFirstProperty;

	void setCurrentItem( QComboBox *pComboBox, char *pszItem );

};

#endif
