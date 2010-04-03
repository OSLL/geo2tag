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
