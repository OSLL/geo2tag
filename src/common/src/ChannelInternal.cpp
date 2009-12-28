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
#include "DataMarkInternal.h"
#include "DbSession.h"

namespace loader
{
  Channel::Channel(unsigned long long id, const std::string &name, const std::string &description, const CHandlePtr<common::DataMarks> &marks): 
                  common::Channel(name, description, marks), m_id(id)
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

  void Channel::addData(CHandlePtr<common::DataMark> mark)
  {
    CHandlePtr<loader::DataMark> m = mark.dynamicCast<loader::DataMark>();
    if(!m)
    {
      // epic fail
      return;
    }
    // update relation betwean mark and channel
    common::DbSession::getInstance().updateChannel(m_id, mark);
    // add mark to Channel object
    common::Channel::addData(mark);
  }

  Channel::~Channel()
  {
  }
} // namespace loader

/* ===[ End of file ]=== */
