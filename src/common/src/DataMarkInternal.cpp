/*! ---------------------------------------------------------------
 *  
 *
 * \file DataMarkInternal.cpp
 * \brief DataMarkInternal implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "DataMarkInternal.h"

namespace loader
{
  DataMark::DataMark(unsigned long long id, double latitude, double longitude, 
             std::string label, std::string description, const std::string& url, const CTime& time,
             const CHandlePtr<common::User> &user, const CHandlePtr<common::Channel> &channel):
              common::DataMark(latitude, longitude, label, description, url, time, user), m_id(id)
    {
      m_channel = channel;
    }
    
  unsigned long long DataMark::getId() const
  {
    return m_id;
  }
  
  void DataMark::setId(unsigned long long id)
  {
    m_id=id;
  }

  void DataMark::setChannel(const CHandlePtr<common::Channel> &channel)
  {
    m_channel = channel;
  }
  
  DataMark::~DataMark()
  {
  }

} // namespace loader

/* ===[ End of file ]=== */
