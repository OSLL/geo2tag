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
/*  */
/*!
 * \file AvailableList.cpp
 * \brief implementation of AvailableList
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */

#include "AvailableListJsonQuery.h"
#include "DbSession.h"
#include <syslog.h>
#include "cast.h"
#include "UserInternal.h"
AvailableList::AvailableList()
{
	m_channels=makeHandle(new common::Channels());
}

void AvailableList::init(const std::stringstream& query){
	json::Element elemRoot;
	std::istringstream s(query.str());
	json::Reader::Read(elemRoot,s); 
	json::QuickInterpreter interpreter(elemRoot);
	const json::String& token=interpreter["auth_token"];
	m_token=std::string(token);
	const json::Number& radius=interpreter["radius"];
	m_radius=radius;
	const json::Number& latitude=interpreter["latitude"];
	m_latitude=latitude;
	const json::Number& longitude=interpreter["longitude"];
	m_longitude=longitude;
}


void AvailableList::process(){
  syslog(LOG_INFO, "m_user=%s m_radius=%f m_latitude=%f m_longitude=%f",m_token.c_str(),m_radius,m_latitude,m_longitude);
  CHandlePtr<std::vector<CHandlePtr<common::User> > > users=common::DbSession::getInstance().getUsers();
  for (std::vector<CHandlePtr<common::User> >::iterator i=users->begin();i!=users->end();i++)
  {
    if ((*i).dynamicCast<loader::User>()->getToken()==m_token)
    {
       m_channels = common::DbSession::getInstance().getChannels();
       break;
    }
  }
}

const CHandlePtr<common::Channels>& AvailableList::getChannels() const
{
	return m_channels;
}

std::string AvailableList::outToString() const
{
	syslog(LOG_INFO,"Starting AvailableList::outToString() method");
	ChannelList  list(m_channels);
	syslog(LOG_INFO,"ChannelList::convertInJSON() method completed");
	return list.getJSON().str();
}

#if 0
void AvailableList::inFromString(const std::string& JSON ){
        json::Element elemRoot;
        std::istringstream iss(JSON);
        json::Reader::Read(elemRoot,iss);
        json::QuickInterpreter interpreter(elemRoot);
        const json::Array& channels=interpreter["channels"];
        int length=channels.Size();
        for (int  i=0;i<length;i++){
                const json::String& name=interpreter["channels"][i]["name"];
//                CHandlePtr<Channel> channel=makeHandle(new Channel(name(),"","",NULL));
//                m_channels->push_back(channel);
        }
}
#endif
/* ===[ End of file  ]=== */
