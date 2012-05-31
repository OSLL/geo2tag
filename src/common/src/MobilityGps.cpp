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
/*! ---------------------------------------------------------------
 *
 *
 * \file MobilityGps.cpp
 * \brief MobilityGps implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "MobilityGps.h"
#include <QDebug>
namespace common
{
  double MobilityGps::m_longitude = 0;
  double MobilityGps::m_latitude = 0;

  MobilityGps::MobilityGps(QObject *parent) : QObject(parent)
  {
    setReady(false);
    m_lastUpdate = QDateTime::currentDateTime();
    QGeoPositionInfoSource *m_source = QGeoPositionInfoSource::createDefaultSource(this);
    if (m_source)
    {
      connect(m_source, SIGNAL(positionUpdated(QGeoPositionInfo)),
        this, SLOT(positionUpdated(QGeoPositionInfo)));
      m_source->startUpdates();
    }
  }

  bool MobilityGps::isReady()
  {
    int ageOfLastCoords = m_lastUpdate.toUTC().secsTo(QDateTime::currentDateTime());
    //    qDebug() << "Age of last received coordinates " << ageOfLastCoords << " secs";
    if (ageOfLastCoords > 60) setReady(false);
    return Gps::isReady();
  }

  void MobilityGps::positionUpdated(QGeoPositionInfo info)
  {
    if (!isReady()) setReady(true);
    //  qDebug() << "Position updated " << info.coordinate().longitude() << " " <<info.coordinate().latitude() ;
    m_longitude = info.coordinate().longitude();
    m_latitude = info.coordinate().latitude();
    m_lastUpdate = QDateTime::currentDateTime();
  }

  double MobilityGps::getLongitude() const
  {
    return m_longitude;
  }

  double MobilityGps::getLatitude() const
  {
    return m_latitude;
  }

  MobilityGps::~MobilityGps()
  {

  }

}                                       // namespace common


/* ===[ End of file ]=== */
