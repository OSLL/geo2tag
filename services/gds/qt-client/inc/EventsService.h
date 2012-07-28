/*
 * Copyright 2012  Ivan Bezyazychnyy  ivan.bezyazychnyy@gmail.com
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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

#ifndef EVENTSSERVICE_H
#define EVENTSSERVICE_H

#include <QObject>
#include <QSet>
#include <QList>
#include <QSharedPointer>
#include "LocationManager.h"
#include "FilterCircleQuery.h"
#include "User.h"
#include <phonon>
#include "Settings.h"

QTM_USE_NAMESPACE

typedef QList<QSharedPointer<DataMark> > Events;
Q_DECLARE_METATYPE(Events)

class EventsService : public QObject
{
  Q_OBJECT

    LocationManager *m_locationManager;
  FilterCircleQuery *m_filterCircleQuery;
  QSharedPointer<common::User> m_user;
  QSet<int> m_eventsIds;
  Phonon::MediaObject *m_mediaPlayer;
  Settings m_settings;
  int m_eventsRadius;
  QSharedPointer<Channel> m_eventsChannel;

  public:
    explicit EventsService(LocationManager *locationManager, QObject *parent = 0);
    ~EventsService();

  public slots:
    void requestEvents();
    void startService(QString name, QString password, QString auth_token, QString serverUrl);
    void stopService();
    void updateSettings();

    signals:
    void eventsReceived(Events events);
    void errorOccured(QString error);

  private slots:
    void onTagsReceived();
    void onError(QString error);

};
#endif                                  // EVENTSSERVICE_H
