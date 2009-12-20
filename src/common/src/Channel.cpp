/*! ---------------------------------------------------------------
 *  
 *
 * \file Channel.cpp
 * \brief Channel implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "Channel.h"

namespace common
{
  Channel::Channel(const std::string &description, const CHandlePtr<DataMarks> &mark):m_description(description), m_marks(marks)
  {
  }

  const std::string& Channel::getDescription() const
  {
    return m_description;
  }

  const CHandlePtr<DataMarks> Channel::getMarks() const
  {
    return m_marks;
  }

  void Channel::addData(CHandlePtr<DataMark> mark)
  {
    m_marks.push_back(mark);
  }

  Channel::~Channel()
  {
  }


  Channels::Channels()
  {
  }

  Channels::~Channels()
  {
  }
} // namespace common

/* ===[ End of file ]=== */
