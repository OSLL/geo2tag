#ifndef CLASSCREDITS_H
#define CLASSCREDITS_H

#include <qwidget.h>
#include <qlayout.h>
#include <qlistbox.h>
#include <qdialog.h>
#include <qpushbutton.h>

class CCredits : public QDialog
{
	Q_OBJECT
public:
    CCredits( QWidget* parent = 0, const char* name = 0 );
	virtual ~CCredits();
};

#endif


