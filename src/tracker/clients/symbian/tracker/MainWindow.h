#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QVBoxLayout>

#include "LoginQuery.h"
#include "AddNewMarkQuery.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
		QLabel * m_status;
		QVBoxLayout * m_layout;
		bool m_authentificated;
		LoginQuery * m_loginQuery;
		AddNewMarkQuery * m_tagQuery;
    
		void sendTag();
		void pause();
	public:
		explicit MainWindow(QWidget *parent = 0);
	public slots:
	  void onAuthentificated();
		void onTagAdded();
		void onError(QString);
};

#endif
