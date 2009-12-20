/*! ---------------------------------------------------------------
 *  
 *
 * \file ChannelInternal.cpp
 * \brief ChannelInternal implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "ChannelInternal.h"

namespace loader
{
  Channel::Channel(unsigned long long id, std::string description, const CHandlePtr<DataMarks> &marks): 
                  common::Channel(description, marks), m_id(id)
  {
  }
  
  unsigned long long Channel::getId() const
  {
    return m_id;
  }
  
  void Channel::setId(unsigned long long id)
  {
    m_id=id;
  }
} // namespace loader

/* ===[ End of file ]=== */
