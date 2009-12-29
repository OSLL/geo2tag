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
#include "DataMarks.h"
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

  DataMarks::DataMarks()
  {
  }

  DataMarks::~DataMarks()
  { 
  }
  
  CHandlePtr<DataMark> DataMark::createMark(double latitude, double longitude, const std::string& label, 
             const std::string& description, const std::string& url, const CTime &time)
  {
    return makeHandle(new loader::DataMark(0,latitude,longitude, label, description, url, time,
              common::DbSession::getInstance().getCurrentUser()));
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
