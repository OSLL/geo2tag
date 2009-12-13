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

#include <stdio.h>
#include <cstring>
#include "DataMarks.h"
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
             const std::string& description, const CTime &time): 
             m_latitude(latitude), 
             m_longitude(longitude), 
             m_label(label),
             m_description(description), 
             m_time(time)
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

  const CTime& DataMark::getTime() const
  {
    return m_time;
  }

  void DataMark::setTime(const CTime& time)
  {
    m_time = time;
  }

  DataMarks::DataMarks()
  {
  }

  void DataMarks::load()
  {
    DbSession session;
    session.loadMarks(*this);
  }

  void DataMarks::save()
  {
    DbSession session;
    session.storeMarks(*this);
  }
  
  DataMarks::~DataMarks()
  {
  }
} // namespace common

/* ===[ End of file  ]=== */
