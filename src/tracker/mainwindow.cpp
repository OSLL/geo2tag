#include "mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QDateTime>
#include <QVBoxLayout>
#include <QTimer>

#include "settingsdialog.h"
#include "LoginDialog.h"

#include "ApplyMarkQuery.h"

#define DEFAULT_LATITUDE 60.17
#define DEFAULT_LONGITUDE 24.95

MainWindow::MainWindow() : QMainWindow(NULL)
{
  QAction *clean = new QAction("&Clean", this);
  QAction *quit = new QAction("&Quit", this);

  QMenu *menu = new QMenu("&Settings",this);
  menu->addAction(clean);
  menu->addAction(quit);
  
  //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  menuBar()->addMenu(menu);
  m_status = new QLabel("Unknown", this);

  QWidget *widget = new QWidget(this);
  widget->setLayout(new QVBoxLayout());
  widget->layout()->addWidget(m_status);
  setCentralWidget(widget);

  connect(clean, SIGNAL(triggered()), this, SLOT(cleanLocalSettigns()));
  connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));

  startGps();
  QTimer::singleShot(0, this, SLOT(setupBearer()));
    
  initSettings();

  startTimer(100); // first update should be fast
}

void MainWindow::cleanLocalSettigns()
{
  QSettings settings("osll","tracker");
  settings.clear();

  QMessageBox::information(this,"Info","All local data was destroyed. Restart application to see effect.");
}

void MainWindow::initSettings()
{
  QSettings settings("osll","tracker");

  if( settings.value("magic").toString() == APP_MAGIC )
  {
    qDebug() << "magic = " << settings.value("magic").toString();
    emit readSettings();
  }
  else
  {
    emit createSettings();
  }
}

void MainWindow::readSettings()
{
  QSettings settings("osll","tracker");
  m_settings.channel = settings.value("channel").toString();
  m_settings.key = settings.value("key").toString();
  m_settings.user = settings.value("user").toString();
  m_settings.passw = settings.value("passwd").toString();
  m_settings.auth_token = settings.value("auth_token").toString();
  m_settings.initialized = true;
}

void MainWindow::createSettings()
{
	int p;
	LoginDialog ld(this);
	if (QDialog::Accepted == ld.exec())
	{
        //p = QMessageBox::question(this,
        //                        "Question","Attach existing channel? (otherwise new one will be created)",//"Create new channel or attach existing?",
        //                        "No", "Yes"); //"Create new", "Attach existing");
        p = 0;
	}
	else
	{
	    p = 1;
	}
	
  SettingsDialog sd(p, this);

  if (QDialog::Accepted == sd.exec())
  {
    m_settings = sd.getData();
    QSettings settings("osll","tracker");
    settings.setValue("channel",m_settings.channel);
    settings.setValue("key",m_settings.key);
    settings.setValue("user",m_settings.user);
    settings.setValue("passwd",m_settings.passw);
    settings.setValue("auth_token",m_settings.auth_token);
    settings.setValue("magic",APP_MAGIC);
    m_settings.initialized = true;
  } else
  {
    m_settings.initialized = false;
  }
}

void MainWindow::startGps()
{
    if (!m_positionSource) {
        m_positionSource = QGeoPositionInfoSource::createDefaultSource(this);
	m_positionSource -> setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
        QObject::connect(m_positionSource, SIGNAL(positionUpdated(QGeoPositionInfo)),
        this, SLOT(positionUpdated(QGeoPositionInfo)));
    }
    m_positionSource->startUpdates();
}

void MainWindow::positionUpdated(QGeoPositionInfo gpsPos)
{
    m_positionInfo = gpsPos;
}

void MainWindow::setupBearer()
{
    // Set Internet Access Point
    QNetworkConfigurationManager manager;
    const bool canStartIAP = (manager.capabilities()
        & QNetworkConfigurationManager::CanStartAndStopInterfaces);
    // Is there default access point, use it
    QNetworkConfiguration cfg = manager.defaultConfiguration();
    if (!cfg.isValid() || !canStartIAP) {
        return;
    }
    m_session = new QNetworkSession(cfg);
    m_session->open();
    m_session->waitForOpened();
}

void MainWindow::timerEvent(QTimerEvent *te)
{
  killTimer(te->timerId());


  if( m_settings.initialized )
  {
    qDebug() << "   ... dropping mark" << QDateTime().currentDateTime();
    bool result = setMark();
  }

  startTimer(UPDATE_INTERVAL);
}

bool MainWindow::setMark()
{
    qreal latitude = DEFAULT_LATITUDE;
    qreal longitude = DEFAULT_LONGITUDE;

    if (m_positionInfo.coordinate().isValid()) {
        latitude = m_positionInfo.coordinate().latitude();
        longitude = m_positionInfo.coordinate().longitude();
        
        GUI::ApplyMarkQuery *applyMarkQuery = new GUI::ApplyMarkQuery
                                                  (m_settings.auth_token,
                                                   m_settings.channel,
                                                   QString("title"),
                                                   QString("url"),
                                                   QString("description"),
                                                   latitude,
                                                   longitude,
                                                   QLocale("english").toString(QDateTime::currentDateTime(),"dd MMM yyyy hh:mm:ss"));
            connect(applyMarkQuery, SIGNAL(responseReceived(QString)), this, SLOT(onApplyMarkResponse(QString)));
            applyMarkQuery->doRequest();
    }
    else {
        qDebug() << "Using wrong coordinates.";
    }

    

    return true;
}

void MainWindow::onApplyMarkResponse(QString status)
{
    QDateTime now = QDateTime::currentDateTime();
    qDebug() << "mark timer " << now;

    if (status == QString("ok"))
    {
        m_status->setText(QString("Last attempt %1, was %2").arg(now.toString()).arg(" success"));
    }
    else
    {
        m_status->setText(QString("Last attempt %1, was %2").arg(now.toString()).arg(" failed"));
    }

}
