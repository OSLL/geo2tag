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

#include <iostream>
#include "Channel.h"

namespace common
{
  Channel::Channel(const std::string &name, const std::string &description, const std::string &url, const CHandlePtr<DataMarks> &mark):
          m_name(name), m_description(description), m_url(url), m_marks(mark), m_isDisplayed(true)
  {
    m_activeRadius = 5.0; // 5 km
  }

  const std::string& Channel::getDescription() const
  {
    return m_description;
  }

  const std::string& Channel::getName() const
  {
    return m_name;
  }

  void Channel::setDescription(const std::string& description)
  {
    m_description = description;
  }

  const std::string& Channel::getUrl() const
  {
    return m_url;
  }
  
  void Channel::setUrl(const std::string& url)
  {
    m_url = url;
  }

  const CHandlePtr<DataMarks> Channel::getMarks() const
  {
    return m_marks;
  }

  void Channel::addData(CHandlePtr<common::DataMark> mark)
  {
    m_marks->push_back(mark);
  }

  bool Channel::isDisplayed() const
  {
    return m_isDisplayed;
  }

  void Channel::setDisplayed(bool fl)
  {
    m_isDisplayed = fl;
  }

  void Channel::setRadius(const double& radius)
  {
    std::cerr << "newRadius "<< radius << " stored" << std::endl;
    m_activeRadius = radius;
  }

  double Channel::getRadius() const
  {
    return m_activeRadius;
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
