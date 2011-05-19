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
 * \file SetSessionPointRequestJSON.h
 * \brief Header of SetSessionPointRequestJSON
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _SETSESSIONPOINTREQUESTJSON_H_70a7918f_a639_4664_bf8a_957ce1be2f0c_INCLUDED_
#define _SETSESSIONPOINTREQUESTJSON_H_70a7918f_a639_4664_bf8a_957ce1be2f0c_INCLUDED_

#include "JsonSerializer.h"

#include <QDateTime>

class SetSessionPointRequestJSON: public JsonSerializer
{
  double m_latitude;
  double m_longitude;
  double m_radius;
  QDateTime m_time;
  qulonglong m_timeSlot;
  bool m_isCurrentTime;

  public:

    SetSessionPointRequestJSON(QObject *parent=0);

    QByteArray getJson() const;

    void parseJson(const QByteArray&);

    double getLatitude() const;
    double getLongitude() const;
    double getRadius() const;
    qulonglong getTimeSlot() const;
    const QDateTime& getTime() const;
    bool getIsTimeCurrent() const;

    void setLatitude(double latitude);
    void setLongitude(double longitude);
    void setRadius(double radius);
    void setTimeSlot(const qulonglong&);
    void setTime(const QDateTime& time/*=QDateTime::currentDateTime().toUTC()*/);
    void setIsTimeCurrent(bool);

};                                      //class SetSessionPointRequestJSON
#endif                                  // _SETSESSIONPOINTREQUESTJSON_H_70a7918f_a639_4664_bf8a_957ce1be2f0c_INCLUDED_
