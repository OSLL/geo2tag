/*
 * Copyright 2010  Open Source & Linux Lab (OSLL) osll@osll.spb.ru
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
 * \file SubscribedList.cpp
 * \brief SubscribedList implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */
#include "User.h"
#include "SubscribedListJsonQuery.h"
#include <syslog.h>
SubscribedList::SubscribedList(const std::stringstream& query)
{
  json::Element elemRoot;
  std::istringstream s(query.str());
  json::Reader::Read(elemRoot,s);
  json::QuickInterpreter interpreter(elemRoot);
  const json::String& user=interpreter["user"];
  m_user=std::string(user);
  m_channels=makeHandle(new common::Channels);
}

void SubscribedList::process()
{
  CHandlePtr<std::vector<CHandlePtr<common::User> > > users=common::DbSession::getInstance().getUsers();
  for (std::vector<CHandlePtr<common::User> >::iterator i=users->begin();i!=users->end();i++){
    syslog(LOG_INFO,"got %s user from DbSession",(*i)->getLogin().c_str());
    if ((*i)->getLogin()==m_user){
      syslog(LOG_INFO,"it is user from query");
      m_channels=(*i)->getSubscribedChannels();
      syslog(LOG_INFO,"got %i channels for %s ",m_channels->size(),(*i)->getLogin().c_str());
      break;
  }
}
  syslog(LOG_INFO,"got %i channels from DbSession",m_channels->size());
}


const CHandlePtr<common::Channels>& SubscribedList::getChannels() const
{
  return m_channels;
}

std::string SubscribedList::outToString() const
{
  ChannelList  list(m_channels);
  return list.getJSON().str();
}


/* ===[ End of file ]=== */
