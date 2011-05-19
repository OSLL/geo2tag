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
 * \file GetSessionPointResponseJSON.h
 * \brief Header of GetSessionPointResponseJSON
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _GETSESSIONPOINTRESPONSEJSON_H_0251a51f_0874_46a2_974c_0adedfc6c781_INCLUDED_
#define _GETSESSIONPOINTRESPONSEJSON_H_0251a51f_0874_46a2_974c_0adedfc6c781_INCLUDED_

#include "JsonSerializer.h"

#include <QString>
#include <QDateTime>

class GetSessionPointResponseJSON: public JsonSerializer
{
  double m_latitude;
  double m_longitude;
  double m_radius;
  QDateTime m_time;
  qulonglong m_timeSlot;

  public:

    GetSessionPointResponseJSON(QObject *parent=0);

    double getLatitude() const;
    double getLongitude() const;
    double getRadius() const;
    qulonglong getTimeSlot() const;
    const QDateTime& getTime() const;

    QByteArray getJson() const;

    void parseJson(const QByteArray&);

};                                      //class GetSessionPointResponseJSON
#endif                                  // _GETSESSIONPOINTRESPONSEJSON_H_0251a51f_0874_46a2_974c_0adedfc6c781_INCLUDED_
