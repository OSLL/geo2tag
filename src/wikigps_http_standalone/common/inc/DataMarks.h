/*
 * Copyright 2010  OSLL osll@osll.spb.ru
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
/*  */
/*!
 * \file DataMarks.h
 * \brief Header of DataMarks
 * \todo add comment here
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _DataMarks_H_E8A2619E_0BF6_4AE8_BB61_F09B92F73637_INCLUDED_
#define _DataMarks_H_E8A2619E_0BF6_4AE8_BB61_F09B92F73637_INCLUDED_

//#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <QDateTime>
#include "common/inc/User.h"
#include "common/inc/Session.h"
#include "common/inc/ConcurrentVector.h"
//#include "Channel.h"

class DataMark: public QObject
{
  Q_OBJECT
    double m_altitude;
  double m_latitude;
  double m_longitude;

  QString m_label;
  QString m_description;
  QString m_url;
  QDateTime m_time;

  QSharedPointer<common::User> m_user;

  QSharedPointer<Session> m_session;

  QSharedPointer<Channel> m_channel;

  public:

    DataMark(double altitude, double latitude, double longitude, QString label,
                                        // TODO add altitude to constructor
      QString description, QString url, QDateTime time);

    void setUser(QSharedPointer<common::User> user);

    void setSession(QSharedPointer<Session> session);

    void setChannel(QSharedPointer<Channel> channel);

  public:

    virtual qlonglong getId() const;

    void setDescription(const QString&);
    const QString& getDescription() const;

    double getLatitude() const;
    void setLatitude(const double&);

    double getLongitude() const;
    void setLongitude(const double&);

    double getAltitude() const;
    void setAltitude(const double&);

    const QString& getLabel() const;
    void setLabel(const QString&);

    const QString& getUrl() const;
    void setUrl(const QString&);

    const QDateTime& getTime() const;
    void setTime(const QDateTime& time=QDateTime::currentDateTime().toUTC());

    QSharedPointer<common::User> getUser() const;

    QSharedPointer<Session> getSession() const;

    QSharedPointer<Channel> getChannel() const;
    static double getDistance(double lat1, double lon1, double lat2, double lon2);

    virtual ~DataMark();
};

bool operator<(const QSharedPointer<DataMark> &a, const QSharedPointer<DataMark> &b);

typedef ConcurrentVector<DataMark> DataMarks;
//_DataMarks_H_E8A2619E_0BF6_4AE8_BB61_F09B92F73637_INCLUDED_
#endif

/* ===[ End of file  ]=== */
