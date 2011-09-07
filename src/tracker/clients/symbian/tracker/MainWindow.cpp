#include "MainWindow.h"
#include "defines.h"
#include "GpsInfo.h"
#include "JsonDataMark.h"
#include "JsonChannel.h"
#include "JsonUser.h"

#include <QEventLoop>
#include <QTimer>

#define TICKING_INTERVAL 5000

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent),m_authentificated(false)
{
  m_layout = new QVBoxLayout;
	m_status = new QLabel(this);
	m_layout->addWidget(m_status);
	setLayout(m_layout);

        m_loginQuery = new LoginQuery(DEFAULT_USER_NAME, DEFAULT_USER_PASSWORD, this);
        connect(m_loginQuery, SIGNAL(connected()), SLOT(onAuthentificated()));
  connect(m_loginQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
  m_status->setText("Authentificating...");
	m_loginQuery->doRequest();

}

void MainWindow::onAuthentificated()
{
	m_status->setText("Authentification succesful");
  m_authentificated = true;
	QSharedPointer<DataMark> mark(new JsonDataMark(common::GpsInfo::getInstance().getLatitude(),
                                                       common::GpsInfo::getInstance().getLongitude(),"t",
			"this tag was generated","unknown",	QDateTime::currentDateTime()));
	QSharedPointer<Channel> channel(new JsonChannel(DEFAULT_CHANNEL,"dummy channel"));
	mark->setChannel(channel);
	mark->setUser(m_loginQuery->getUser());
	m_tagQuery = new AddNewMarkQuery(mark,this);
	connect(m_tagQuery, SIGNAL(tagAdded()), SLOT(onTagAdded()));
	connect(m_tagQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
	m_tagQuery->doRequest();

}

void MainWindow::sendTag()
{
	m_tagQuery->getTag()->setTime(QDateTime::currentDateTime());
	m_tagQuery->getTag()->setLatitude(common::GpsInfo::getInstance().getLatitude());
	m_tagQuery->getTag()->setLongitude(common::GpsInfo::getInstance().getLongitude());

  m_tagQuery->doRequest();
}

void MainWindow::pause()
{
	QEventLoop eventLoop;
  QTimer::singleShot(TICKING_INTERVAL, &eventLoop, SLOT(quit())); 
	eventLoop.exec();
}

void MainWindow::onTagAdded()
{
	m_status->setText(QString("Tag added succesfuly at:")+QDateTime::currentDateTime().toString());

	pause();
  sendTag();
}

void MainWindow::onError(QString error)
{
  m_status->setText(QString("Error occured:") + error );
	// If authentification is not succesful do it again
	if(!m_authentificated)
	{
		pause();
		m_loginQuery->doRequest();
	}	else sendTag(); 
}
