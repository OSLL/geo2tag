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

#ifndef FILTERCIRCLEQUERY_H
#define FILTERCIRCLEQUERY_H

#include <QObject>
#include <QString>
#include <QDateTime>

#include "DefaultQuery.h"
#include "DataMarks.h"
#include "User.h"
#include "Channel.h"
#include "DataChannel.h"

class FilterCircleQuery : public DefaultQuery
{
  Q_OBJECT

    QSharedPointer<common::User> m_user;
  double m_latitude;
  double m_longitude;
  double m_radius;
  QDateTime m_timeFrom;
  QDateTime m_timeTo;
  QSharedPointer<Channel> m_channel;

  DataChannels m_hashMap;

  virtual QString getUrl() const;
  virtual QByteArray getRequestBody() const;

  private slots:

    virtual void processReply(QNetworkReply *reply);

  public:

    FilterCircleQuery(QSharedPointer<common::User> &user,
      double latitude,
      double longitude,
      double radius,
      QDateTime timeFrom,
      QDateTime timeTo,
      const QSharedPointer<Channel> &channel = QSharedPointer<Channel>(0),
      QObject *parent = 0);

    FilterCircleQuery(QObject *parent = 0);

    void setQuery(QSharedPointer<common::User> &user,
      double latitude,
      double longitude,
      double radius,
      QDateTime timeFrom,
      QDateTime timeTo,
      const QSharedPointer<Channel> &channel = QSharedPointer<Channel>(0));

    ~FilterCircleQuery();

    const DataChannels& getData() const;

    signals:

    void tagsReceived();

    // class LoadTagsQuery
};
#endif                                  /* FILTERCIRCLEQUERY_H */
