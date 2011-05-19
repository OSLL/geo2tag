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
 *
 * \file Session.cpp
 * \brief Session implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "Session.h"

Session::Session():
m_latitude(60.0),
m_longitude(30.0),
m_radius(20037.85),
m_timeSlot(525600),
m_isCurrentTime(1),
m_time(QDateTime())
{
}


Session::Session(double latitude,
double longitude,
double radius,
qulonglong timeSlot,
bool isCurrentTime,
QDateTime time):
m_latitude(latitude),
m_longitude(longitude),
m_radius(radius),
m_timeSlot(timeSlot),
m_isCurrentTime(isCurrentTime)
{
  if (!isCurrentTime)
    m_time = time;
  else
    m_time = QDateTime();
}


double Session::getLatitude() const
{
  return m_latitude;
}


void Session::setLatitude(const double& latitude)
{
  m_latitude = latitude;
}


double Session::getLongitude() const
{
  return m_longitude;
}


void Session::setLongitude(const double& longitude)
{
  m_longitude = longitude;
}


double Session::getRadius() const
{
  return m_radius;
}


void Session::setRadius(const double& radius)
{
  m_radius = radius;
}


qulonglong Session::getTimeSlot() const
{
  return m_timeSlot;
}


void Session::setTimeSlot(const qulonglong& timeSlot)
{
  m_timeSlot = timeSlot;
}


QDateTime Session::getTime() const
{
  if (m_isCurrentTime)
    return QDateTime::currentDateTime().toUTC();
  return m_time;
}


void Session::setTime(const QDateTime& time)
{
  m_time = time;
}


bool Session::getIsTimeCurrent() const
{
  return m_isCurrentTime;
}


void Session::setIsTimeCurrent(bool vl)
{
  m_isCurrentTime = vl;
}
