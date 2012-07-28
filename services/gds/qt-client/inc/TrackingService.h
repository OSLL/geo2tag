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

#ifndef TRACKINGSERVICE_H
#define TRACKINGSERVICE_H

#include <QPointF>
#include "WriteTagQuery.h"
#include "LocationManager.h"
#include "DataMarks.h"
#include "Settings.h"

class TrackingService : public QObject
{
  Q_OBJECT

    WriteTagQuery *m_writeTagQuery;
  LocationManager *m_locationManager;
  QSharedPointer<DataMark> m_dataMark;
  QSharedPointer<common::User> m_user;
  QSharedPointer<Channel> m_channel;
  Settings m_settings;
  int m_period;

  public:
    explicit TrackingService(LocationManager *locationManager, QObject *parent = 0);

  public slots:
    void startTracking(QString name, QString password, QString authToken, QString serverUrl);
    void stopTracking();
    void updateSettings();

    signals:
    void markSent(QPointF coordinates);
    void errorOccured(QString error);

  private slots:
    void sendMark();
    void onMarkSent();
    void onError(QString error);

};
#endif                                  // TRACKINGSERVICE_H
