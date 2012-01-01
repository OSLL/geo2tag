/*
 * Copyright 2010-2011  OSLL osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
/*!
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include <QTimer>
#include <QDateTime>
#include "MainWindow.h"
#include <QLabel>
#ifndef Q_WS_SYMBIAN
#include <stdio.h>
#endif
#include "DaemonManager.h"
#define BUFSIZE 1024

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
m_isServiceStarted(false)
{
  setupUi(this);

  connect(startButton, SIGNAL(clicked()), SLOT(startButtonClicked()));
  connect(logButton, SIGNAL(clicked()), SLOT(logButtonClicked()));
  connect(settingsButton, SIGNAL(clicked()), SLOT(settingsButtonClicked()));
  connect(aboutButton, SIGNAL(clicked()), SLOT(aboutButtonClicked()));
  connect(daemonRestartButton, SIGNAL(clicked()),SLOT(restartDaemon()));

  QTimer *timer = new QTimer(this);
  connect(timer,SIGNAL(timeout()), SLOT(checkDaemon()));
  timer->start(5000);

  m_optionsWidget = new OptionsWidget("tracker",this);
  m_logWidget = new LogWidget(this);
  m_aboutWidget = new AboutWidget(this);
  m_stackedWidget->addWidget(m_optionsWidget);
  m_stackedWidget->addWidget(m_logWidget);
  m_stackedWidget->addWidget(m_aboutWidget);
  connect(m_optionsWidget, SIGNAL(done()), SLOT(doneButtonClicked()));
  connect(m_logWidget, SIGNAL(done()), SLOT(moveToFirstPage()));
  connect(m_aboutWidget, SIGNAL(done()), SLOT(moveToFirstPage()));
  connect(m_optionsWidget, SIGNAL(cancel()), SLOT(moveToFirstPage()));
  //    setStyleSheet( "background-color: rgba( 255, 255, 255, 0% );" );
}


void MainWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type())
  {
    case QEvent::LanguageChange:
      retranslateUi(this);
      break;
    default:
      break;
  }
}


void MainWindow::doneButtonClicked()
{
  DaemonManager::getInstance().reload();
  moveToFirstPage();
}


void MainWindow::startButtonClicked()
{
  m_isServiceStarted = !m_isServiceStarted;
  if(!m_isServiceStarted)
  {
    startButton->setIcon(QIcon(":/images/start-256.png"));
    DaemonManager::getInstance().stop();
  }
  else
  {
    startButton->setIcon(QIcon(":/images/stop-256.png"));
    DaemonManager::getInstance().start();
  }

}


void MainWindow::logButtonClicked()
{
  m_stackedWidget->setCurrentWidget(m_logWidget);
}


void MainWindow::aboutButtonClicked()
{
  m_stackedWidget->setCurrentWidget(m_aboutWidget);
}


void MainWindow::settingsButtonClicked()
{
  m_stackedWidget->setCurrentWidget(m_optionsWidget);
}


void MainWindow::moveToFirstPage()
{
  m_stackedWidget->setCurrentIndex(0);
}


void MainWindow::readData()
{

  if (DaemonManager::getInstance().isStarted())
  {
    QPointF lastCoords=DaemonManager::getInstance().getLastCoordinates();
    QString buf=QString::number(lastCoords.x())+","+QString::number(lastCoords.y());
    m_daemonIndicator->setText("Tracker running");
    startButton->setIcon(QIcon(":/images/stop-256.png"));
    if (buf!="0,0")
      m_logWidget->addToLog(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss.zzz")+": mark placed at "+buf);
    qDebug() << "text now " <<m_daemonIndicator->text();
    m_isServiceStarted = true;
  }else
  {
    m_daemonIndicator->setText("Tracker stopped");
    startButton->setIcon(QIcon(":/images/start-256.png"));
    m_isServiceStarted = false;
  }

}


void MainWindow::restartDaemon()
{
  DaemonManager::getInstance().restart();
}


void MainWindow::checkDaemon()
{
  if(DaemonManager::getInstance().isConnected())
  {
    // TODO investigate what does it mean
    //    updateData();
  }
  else m_logWidget->addToLog(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss.zzz")+": can't connect to daemon");
}
