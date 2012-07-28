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
#include <syslog.h>
#include <algorithm>

namespace common
{

  User::User(const QString &name, const QString &passw, const QString &email)
    : BasicUser(name, passw, email),
    m_channels(new Channels())
  {
  }

  qlonglong User::getId() const
  {
    // Database is not contain 0 in sequences, see scripts/base.sql
    return 0;
  }

  void User::subscribe(const QSharedPointer<Channel>& channel)
  {
    //    syslog(LOG_INFO,"Trying subscribed %lld for %lld",getId(),channel->getId());
    //   if(!m_channels->exist(channel->getId()))
    //   {
    //syslog(LOG_INFO, "User->Subscribe: Num of channels before: %d", m_channels->size());
    m_channels->push_back(channel);
    //syslog(LOG_INFO, "User->Subscribe: Num of channels after: %d", m_channels->size());
    //syslog(LOG_INFO,"Success subscription");
    //    }else
    //    {
    //      syslog(LOG_INFO,"Unsuccess subscription");
    //    }
  }

  void User::unsubscribe(const QSharedPointer<Channel>& channel)
  {
    //syslog(LOG_INFO, "User->Unsubscribe: Num of channels before: %d", m_channels->size());
    m_channels->erase(channel);
    //syslog(LOG_INFO, "User->Unsubscribe: Num of channels after: %d", m_channels->size());
  }

  const QSharedPointer<Channels> User::getSubscribedChannels() const
  {
    return m_channels;
  }

  User::~User()
  {
  }

}                                       // namespace common


/* ===[ End of file ]=== */
