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

#ifndef MEEGOCLIENT_H_
#define MEEGOCLIENT_H_

#include <QTimer>
#include <QSharedPointer>
#include <QString>
#include <QNetworkConfigurationManager>

#include "LoginQuery.h"
#include "WriteTagQuery.h"

#include "markshistory.h"

class MeegoClient: public QObject
{
  Q_OBJECT

    int m_trackInterval;
  bool m_authentificated;

  QString m_lastError;
  QTimer * m_timer;
  QString m_name;

  LoginQuery * m_loginQuery;
  WriteTagQuery * m_addNewMarkQuery;
  MarksHistory * m_history;

  QSharedPointer<common::User> m_user;
  QNetworkConfigurationManager * m_netManager;
  void pause(int msecs);
  public:
    MeegoClient(QObject * parent=0);
    // stub for authentification mechanizm
    void auth(QString user, QString pass);

    void startTrack();
    void stopTrack();
    bool isTracking();

    void setTrackInterval(int sec);
    int getTrackInterval();

    // default 50, maximum 200;
    void setHistoryLimit(int maxMarks);
    int getHistoryLimit();
    void sendHistory();

    bool isAuthentificated();
    void setDispName(const QString & name );
    MarksHistory * getAllMarks();

    QString getLastError();

    bool isOnline();

  private slots:
    void onError(QString error);
    void onAuthentificated();
    void onMarkAdded();
    void track();

  public slots:
    // Network going down|up
    void onNetworkEvent(bool state);
    // on closed || on low battery
    void onGoOffEvent();
    // Send last added coordinate
    void sendLastCoordinate();
    // When history is full
    void onHistoryFull();
    signals:
    void error(QString error);
    void authentificated();
};
#endif
