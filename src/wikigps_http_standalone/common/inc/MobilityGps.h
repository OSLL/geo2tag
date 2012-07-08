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
/*!
 * \file MobilityGps.h
 * \brief Header of MobilityGps
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _MobilityGps_H_21C729E1_9D6C_4EE5_87CC_A3579FDA1E3C_INCLUDED_
#define _MobilityGps_H_21C729E1_9D6C_4EE5_87CC_A3579FDA1E3C_INCLUDED_

#include "GpsInfo.h"
#include <QObject>
#include <QDateTime>

// QtMobility API headers
// Location
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>

// QtMobility namespace
QTM_USE_NAMESPACE

namespace common
{
  /*!
   * Class description. May use HTML formatting
   *
   */
  class MobilityGps: public QObject, public Gps
  {
    Q_OBJECT

      static double m_longitude;
    static double m_latitude;
    QDateTime m_lastUpdate;

    QGeoPositionInfoSource * m_source;
    public:
      MobilityGps(QObject *parent = 0);

      virtual double getLongitude() const;

      virtual double getLatitude() const;

      virtual bool isReady();

      virtual ~MobilityGps();

    private slots:
      void positionUpdated(QGeoPositionInfo info);

    private:
      MobilityGps(const MobilityGps& obj);
      MobilityGps& operator=(const MobilityGps& obj);

      // class MobilityGps
  };

  // namespace common
}


//_MobilityGps_H_21C729E1_9D6C_4EE5_87CC_A3579FDA1E3C_INCLUDED_
#endif

/* ===[ End of file ]=== */
