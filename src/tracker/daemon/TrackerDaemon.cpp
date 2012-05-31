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

#include "TrackerDaemon.h"
#include <QSettings>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QEventLoop>

#include "JsonDataMark.h"
#include "JsonChannel.h"
#include "JsonUser.h"
#include "LoginQuery.h"
#include "AddNewMarkQuery.h"

#ifndef Q_WS_SYMBIAN
#include "ReportThread.h"
#endif

#include "defines.h"
#include "SettingsStorage.h"

#define DAEMON_PORT 34243

TrackerDaemon::TrackerDaemon():
m_settings(QSettings::SystemScope,"osll","tracker"),
m_netManager(this),
m_loginQuery(NULL),
m_tagQuery(NULL),
m_pauseFlag(true),
m_isConnected(false)
{
  #ifndef NO_DAEMON
  m_controlServer = new QTcpServer(NULL);
  connect(m_controlServer, SIGNAL(newConnection()),SLOT(newControlConnection()));
  m_controlServer->listen(QHostAddress::LocalHost, 31234);
  #endif
  connect(&m_netManager,SIGNAL(onlineStateChanged(bool)),SLOT(onOnlineChanged(bool)));

}


void TrackerDaemon::onOnlineChanged(bool state)
{
  if( state )
  {
    qDebug() << "Online now";
    if ( !m_pauseFlag )
    {
      onTagAdded();
    }
  }
  else qDebug() << "Offline now";
}


#ifndef NO_DAEMON
void TrackerDaemon::newControlConnection()
{
  qDebug() << "GUI connected to the control socket";
  while(m_controlServer->hasPendingConnections())
  {
    new ControlThread(m_controlServer->nextPendingConnection(),this,this);
  }
}
#endif

void TrackerDaemon::run()
{
  qDebug() << "Thread started";
  QEventLoop eventLoop;
  QString login = m_settings.value("user").toString();
  QString password = m_settings.value("password").toString();

  m_channelName = m_settings.value("channel").toString();
  m_visibleName = m_settings.value("visibleName").toString();

  qDebug() << "Read from QSettings " << login << " ,"<< password << " ," <<m_channelName;

  SettingsStorage storage(SETTINGS_STORAGE_FILENAME);
  QString tracker_tag_label = storage.getValue("Tracker_Settings/tracker_tag_label").toString();

  if(m_visibleName.isEmpty())
    m_visibleName = tracker_tag_label;

  if(login.isEmpty())
    login = DEFAULT_USER_NAME;

  if(password.isEmpty())
    password = DEFAULT_USER_PASSWORD;

  if(m_channelName.isEmpty())
    m_channelName = DEFAULT_CHANNEL;

  m_loginQuery = new LoginQuery(login, password, this);

  connect(m_loginQuery, SIGNAL(connected()), SLOT(onConnected()));
  connect(m_loginQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));

  while(!m_netManager.isOnline())
  {
    qDebug() << "Device is offline, waiting for network connection";
    QTimer::singleShot(5000, &eventLoop, SLOT(quit()));
    eventLoop.exec();
  }

  m_loginQuery->doRequest();
  qDebug() << "Sent first LoginRequest";
}


void TrackerDaemon::reloadSettings()
{
  qDebug() << "Doing reload of auth_token";
  m_settings.sync();
  m_isConnected = false;
  m_pauseFlag = true;
  run();
}


void TrackerDaemon::startTracking()
{
  qDebug() << "Starting tracker daemon";
  m_pauseFlag = false;

  onTagAdded();
}


void TrackerDaemon::stopTracking()
{
  qDebug() << "Stoping tracker daemon";
  m_pauseFlag = true;
}


bool TrackerDaemon::isTracking() const
{
  return !m_pauseFlag;
}


QStringList TrackerDaemon::getLog() const
{
  return QStringList("dummy");
}


void TrackerDaemon::onError(QString message)
{
  qDebug() << "Network error occured: " << message;
  if (!m_isConnected)
  {
    QEventLoop eventLoop;
    QTimer::singleShot(5000, &eventLoop, SLOT(quit())); eventLoop.exec();
    qDebug() << "Trying to login one more time";
    if (m_netManager.isOnline())
    {
      qDebug() << "Current network state is Online, runing loginRequest again";
      run();
    }
    else
    {
      qDebug() << "Current network state is Offline, wait for connection";
    }
  }
  else if (!m_pauseFlag)
  {
    qDebug() << "Doing another try for add tag";
    onTagAdded();
  }
}


void TrackerDaemon::onConnected()
{
  m_isConnected = true;

  qDebug() << "Auth_token received - " << m_loginQuery->getUser()->getToken() ;
  if(m_tagQuery == NULL)
  {
    QSharedPointer<DataMark> mark(new JsonDataMark(0.,0.,
    //DEFAULT_LATITUDE,DEFAULT_LONGITUDE,
      m_visibleName,
      "this tag was generated automaticaly by tracker application",
      "unknown",
      QDateTime::currentDateTime()));
    m_lastCoords.setX(0.);
    m_lastCoords.setY(0.);
    QSharedPointer<Channel> channel(new JsonChannel(m_channelName,"dummy channel"));
    mark->setChannel(channel);
    mark->setUser(m_loginQuery->getUser());
    m_tagQuery = new AddNewMarkQuery(mark,this);
    connect(m_tagQuery, SIGNAL(tagAdded()), SLOT(onTagAdded()));
    connect(m_tagQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
    onTagAdded();
  }
}


void TrackerDaemon::onTagAdded()
{
  qDebug() << "TrackerDaemon::onTagAdded";

  QEventLoop eventLoop;
  QTimer::singleShot(5000, &eventLoop, SLOT(quit())); eventLoop.exec();

  if(!m_pauseFlag && m_isConnected)
  {
    qDebug() << "Setting and adding new tag";

    if(m_tagQuery)
    {
      while (!common::GpsInfo::getInstance().isReady() || !m_netManager.isOnline())
      {
        qDebug() << "Position source doesnt ready or there is no internet connection, waiting";
        qDebug() << "Position source ready " << common::GpsInfo::getInstance().isReady();

        m_lastCoords.setX(0);
        m_lastCoords.setY(0);

        eventLoop.processEvents(QEventLoop::ExcludeUserInputEvents, 1000);
        QTimer::singleShot(5000, &eventLoop, SLOT(quit()));
        eventLoop.exec();
      }

      m_tagQuery->getTag()->setTime(QDateTime::currentDateTime());
      m_tagQuery->getTag()->setLatitude(common::GpsInfo::getInstance().getLatitude());
      m_tagQuery->getTag()->setLongitude(common::GpsInfo::getInstance().getLongitude());
      m_lastCoords.setX(common::GpsInfo::getInstance().getLatitude());
      m_lastCoords.setY(common::GpsInfo::getInstance().getLongitude());

      qDebug() << "Tag parameters: time " <<  m_tagQuery->getTag()->getTime().toString("dd.MM.yyyy hh:mm:ss.zzz")
        << ", latitude " << m_tagQuery->getTag()->getLatitude()
        << ", longitude " << m_tagQuery->getTag()->getLongitude();

      m_tagQuery->doRequest();
    }
  }
  else
  {
    qDebug() << "Nothing to do: m_pauseFlag ==" << m_pauseFlag << ", m_isConnected ==" << m_isConnected;
  }
}


const QPointF& TrackerDaemon::getLastCoords() const
{
  return m_lastCoords;
}


TrackerDaemon::~TrackerDaemon()
{
  #ifndef NO_DAEMON
  delete m_controlServer;
  #endif
}
