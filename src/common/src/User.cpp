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
 * \file User.cpp
 * \brief User implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "User.h"
#include <algorithm>
namespace common
{
  User::User(const std::string& name, const std::string& pass): m_login(name), m_password(pass), m_channels(makeHandle(new Channels()))
  {
  }
  
  void User::subscribe(const CHandlePtr<Channel>& channel)
  {
    //!> ToDo: check that this channel does not exist in m_channels
    m_channels->push_back(channel);
  }

  void User::unsubscribe(const CHandlePtr<Channel>& channel)
  {
    Channels::iterator it = std::find(m_channels->begin(), m_channels->end(), channel);
    if(it != m_channels->end())
       m_channels->erase(it);
  }
  
  const std::string& User::getLogin() const
  {
    return m_login;
  }

  const std::string& User::getToken() const
  {
    return m_token;
  }

  const std::string& User::getPassword() const
  {
    return m_password;
  }
  
  const CHandlePtr<Channels> User::getSubscribedChannels() const
  {
    return m_channels;
  }

  void User::setPassword(std::string password)
  {
      m_password = password;
  }

  User::~User()
  {
  }
} // namespace common

/* ===[ End of file ]=== */
