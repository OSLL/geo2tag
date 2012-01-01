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

#ifndef TRACKERDAEMON_H
#define TRACKERDAEMON_H

#include <QObject>
#include <QDateTime>
#include <QThread>
#include <QSettings>
#include <QTextStream>
#include <QFile>
#include "GpsInfo.h"
#include "tracker.h"
#include "QTcpServer"
#include "LoginQuery.h"
#include "AddNewMarkQuery.h"
#include <QTimer>
#include <QPointF>
#include <QNetworkConfigurationManager>

#ifndef Q_WS_SYMBIAN
#include "Control.h"
#define PARENT Control
#else
#include <QThread>
#define PARENT QThread
#endif

class TrackerDaemon : /*public QThread,*/ public PARENT
{
  Q_OBJECT

    QSettings m_settings;
  QString m_channelName;
  QString m_visibleName;
  QPointF m_lastCoords;
  QNetworkConfigurationManager m_netManager;

  // this field needs because query is asynchronous
  LoginQuery * m_loginQuery;
  // this field needs because query is asynchronous
  AddNewMarkQuery * m_tagQuery;

  bool m_pauseFlag;
  bool m_isConnected;
  #ifndef NO_DAEMON
  QTcpServer * m_controlServer;
  #endif
  public:
    void run();
  private slots:

    //calls when login/pass were accepted
    void onConnected();
    void onTagAdded();
    void onError(QString);

    void onOnlineChanged(bool state);
  #ifndef NO_DAEMON
    void newControlConnection();
  #endif
  public:
    TrackerDaemon();

    virtual void stopTracking();
    virtual void startTracking();
    virtual void reloadSettings();
    virtual bool isTracking() const;
    const QPointF& getLastCoords() const;

    QStringList getLog() const;

    ~TrackerDaemon();

  public slots:

};
//#endif

// TRACKERDAEMON_H
#endif
