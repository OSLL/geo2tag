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
 * \file DataMarks.cpp
 * \brief DataMarks implementation
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */

#include <math.h>
#include <stdio.h>
#include <cstring>
#include "DataMarkInternal.h"
#include "DbSession.h"

namespace common
{
  void DataMark::setDescription(const std::string& s)
  {
    m_description = s;
  }
  
  const std::string& DataMark::getDescription() const
  {
    return m_description;
  }

  DataMark::DataMark(double latitude, double longitude, const std::string& label, 
             const std::string& description, const std::string& url, const CTime &time,
             const CHandlePtr<User> &user): 
             m_latitude(latitude), 
             m_longitude(longitude), 
             m_label(label),
             m_description(description), 
      	     m_url(url),
             m_time(time),
             m_user(user)
  {
  }

  DataMark::~DataMark()
  {

  }

  double DataMark::getLatitude() const
  {
    return m_latitude;
  }

  void DataMark::setLatitude(const double& lat)
  {
    m_latitude = lat;
  }
  
  double DataMark::getLongitude() const
  {
    return m_longitude;
  }

  void DataMark::setLongitude(const double &lon)
  {
    m_longitude = lon;
  }

  const std::string& DataMark::getLabel() const
  {
    return m_label;
  }

  void DataMark::setLabel(const std::string& label)
  {
    m_label = label;
  }

  const std::string& DataMark::getUrl() const
  {
    return m_url;
  }

  void DataMark::setUrl(const std::string& url)
  {
    m_url = url;
  }

  const CTime& DataMark::getTime() const
  {
    return m_time;
  }

  void DataMark::setTime(const CTime& time)
  {
    m_time = time;
  }
  
  CHandlePtr<User> DataMark::getUser() const
  {
    return m_user;
  }

  CHandlePtr<Channel> DataMark::getChannel() const
  {
    return m_channel;
  }

  DataMarks::DataMarks()
  {
  }

  DataMarks::~DataMarks()
  { 
  }
  
  CHandlePtr<DataMark> DataMark::createMark(double latitude, double longitude, const std::string& label, 
             const std::string& description, const std::string& url, const CTime &time, const CHandlePtr<Channel> &channel)
  {
    return makeHandle(new loader::DataMark(0,latitude,longitude, label, description, url, time,
              common::DbSession::getInstance().getCurrentUser(), channel));
  }


  double deg2rad(double deg) 
  {
      return (deg * M_PI / 180);
  }

  double rad2deg(double rad) 
  {
      return (rad * 180 / M_PI);
  }
  double DataMark::getDistance(double lat1, double lon1, double lat2, double lon2)
  {
    double theta, dist;
    theta = lon1 - lon2;
    dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
    dist = acos(dist);
    dist = rad2deg(dist);
    dist = dist * 60 * 1.1515; // in miles
    dist = dist * 1.609344; // in kilometers
    return (dist);
  }
} // namespace common

/* ===[ End of file  ]=== */
