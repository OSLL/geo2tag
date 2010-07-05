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
/*  */
/*!
 * \file ChannelList.cpp
 * \brief implementation of Channels2JSON
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */

#include "ChannelList.h"
#include "DataMarks.h"
#include <syslog.h>
#include <assert.h>

namespace stub
{
  class Channel: public common::Channel
  {
  public:
    Channel(const std::string& name, const std::string& description): common::Channel(name,description, "not available", makeHandle(new common::DataMarks()))
    {
    }

    virtual ~Channel()
    {
    }
  };
}

ChannelList::ChannelList(const CHandlePtr<common::Channels>& channels):m_channels(channels){
	convertInJSON();
}

ChannelList::ChannelList(const std::stringstream& json){
    m_channels=makeHandle(new common::Channels);
    m_json << json.str();
    convertInChannels();
}

void ChannelList::convertInJSON(){
        // syslog(LOG_INFO,"Starting ChannelList::convertInJson() method");
	assert(m_channels);
  json::Element newRoot;
  json::QuickBuilder builder(newRoot);
  json::QuickBuilder channels=builder["channels"];
	int j=0;
  if(m_channels->size()>0)
  {
	  for (common::Channels::iterator i=m_channels->begin();i!=m_channels->end();i++ ){
	  	channels[j]["name"]=json::String((*i)->getName().c_str());
  		channels[j]["description"]=json::String((*i)->getDescription());
	  	channels[j]["tags"][0]=json::String("");
	  	j++;
	  }
  }
  else
  {
    channels = json::Null();
  }
	json::Writer::Write(newRoot,m_json);
}

void ChannelList::convertInChannels(){
    json::Element elemRoot;
    json::Reader::Read(elemRoot,m_json);
    json::QuickInterpreter interpreter(elemRoot);
    try {
      const json::Array& channels=interpreter["channels"];
      int length=channels.Size();
      for (int  i=0;i<length;i++){
          const json::String& name=interpreter["channels"][i]["name"];
          const json::String& description=interpreter["channels"][i]["description"];
          CHandlePtr<common::Channel> channel=makeHandle(new stub::Channel(std::string(name), std::string(description)));
          m_channels->push_back(channel);
      }
    } catch (const json::Exception& x)
    {
      //array it isn't an array;
    }
}

const std::stringstream& ChannelList::getJSON() const {
	return m_json;
}

const CHandlePtr<common::Channels>& ChannelList::getChannels() const{
	return m_channels;
}
/* ===[ End of file  ]=== */
