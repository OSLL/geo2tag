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

#include "settingsdialog.h"

#include "ApplyMarkQuery.h"

MainWindow::MainWindow() : QMainWindow(NULL)
{
  QAction *clean = new QAction("&Clean", this);
  QAction *quit = new QAction("&Quit", this);

  QMenu *menu = new QMenu("&Settings",this);
  menu->addAction(clean);
  menu->addAction(quit);

  menuBar()->addMenu(menu);
  m_status = new QLabel("Unknown", this);

  QWidget *widget = new QWidget(this);
  widget->setLayout(new QVBoxLayout());
  widget->layout()->addWidget(m_status);
  setCentralWidget(widget);

  connect(clean, SIGNAL(triggered()), this, SLOT(cleanLocalSettigns()));
  connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));


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
  int p = QMessageBox::question(this,
                                "Question","Create new channel or attach existing?",
                                "Create new", "Attach existing");

  SettingsDialog sd(p,this);

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
    GUI::ApplyMarkQuery *applyMarkQuery = new GUI::ApplyMarkQuery
                                          (m_settings.auth_token,
                                           m_settings.channel,
                                           QString("title"),
                                           QString("url"),
                                           QString("description"),
                                           60.17, 24.95,
                                           QLocale("english").toString(QDateTime::currentDateTime(),"dd MMM yyyy hh:mm:ss"));
    connect(applyMarkQuery, SIGNAL(responseReceived(QString)), this, SLOT(onApplyMarkResponse(QString)));
    applyMarkQuery->doRequest();

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
