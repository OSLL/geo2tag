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
#ifndef CFileSelector_included
#define CFileSelector_included

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qfiledialog.h>

class CFileSelector : public QWidget
{
    Q_OBJECT

public:

    CFileSelector( QWidget* parent = NULL, const char* name = NULL );
    ~CFileSelector();

	QLineEdit	*pLineEdit;
	QPushButton	*pButton;

protected slots:
	void pButton_Clicked();

protected:

};
#endif 
