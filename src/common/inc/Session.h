/*
 * Copyright ${2011}  ${Tatiana Trofimova}  ${trotava@gmail.com}
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

/*! ---------------------------------------------------------------
 * \file Session.h
 * \brief Header of Session
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _SESSION_H_27cc31d1_34fc_435a_8d41_283814c36daa_INCLUDED_
#define _SESSION_H_27cc31d1_34fc_435a_8d41_283814c36daa_INCLUDED_

/*!
 * Class description.
 *
 */

#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <QDateTime>

#include "ConcurrentVector.h"

class Session: public QObject
{
  Q_OBJECT;

  double m_latitude;                    // latitude of centre of zone for drawing tags
  double m_longitude;                   // longitude of centre of zone for drawing tags
  double m_radius;                      // radius of zone for drawing tags (in km)
  qulonglong m_timeSlot;                // 'radius' of time zone for drawing tags (in minutes)
  bool m_isCurrentTime;
  QDateTime m_time;                     // 'centre' of time zone for drawing tags

  public:
    Session();

    Session(double latitude, double longitude, double radius, qulonglong timeSlot, bool isCurrentTime, QDateTime time);

    double getLatitude() const;
    void setLatitude(const double&);

    double getLongitude() const;
    void setLongitude(const double&);

    double getRadius() const;
    void setRadius(const double&);

    qulonglong getTimeSlot() const;
    void setTimeSlot(const qulonglong&);

    QDateTime getTime() const;
    void setTime(const QDateTime&);

    bool getIsTimeCurrent() const;
    void setIsTimeCurrent(bool);

  private:
    Session(const Session&);
    Session& operator=(const Session&);

};                                      // class Session

typedef ConcurrentVector<Session> Sessions;
#endif                                  // _SESSION_H_27cc31d1_34fc_435a_8d41_283814c36daa_INCLUDED_
