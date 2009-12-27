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
             const CHandlePtr<common::User> &user):
              common::DataMark(latitude, longitude, label, description, url, time, user), m_id(id)
    {
    }
    
  unsigned long long DataMark::getId() const
  {
    return m_id;
  }
  
  void DataMark::setId(unsigned long long id)
  {
    m_id=id;
  }
  
  DataMark::~DataMark()
  {
  }

} // namespace loader

/* ===[ End of file ]=== */
