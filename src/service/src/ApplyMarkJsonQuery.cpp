/*
 * Copyright <year>  <name of author> <e-mail>
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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
 * $Id$ 
 *
 * \file ApplyMarkJsonQuery.cpp
 * \brief ApplyMarkJsonQuery implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "ApplyMarkJsonQuery.h"
#include "ChannelInternal.h"
#include "DataMarkInternal.h"
#include "UserInternal.h"
#include <time.h>
#include "DbSession.h"
#include "Time.h"
#include <syslog.h>
ApplyMarkJsonQuery::ApplyMarkJsonQuery(){

}

void ApplyMarkJsonQuery::init(const std::stringstream& query){

	json::Element elemRoot;
	std::istringstream s(query.str());
	json::Reader::Read(elemRoot,s);
	json::QuickInterpreter interpreter(elemRoot);
	const json::String& token=interpreter["auth_token"];
	m_token=std::string(token);
	const json::String& channel=interpreter["channel"];
	m_channel=std::string(channel);
        const json::Number& latitude=interpreter["latitude"];
	m_latitude=latitude;
        const json::Number& longitude=interpreter["longitude"];
	m_longitude=longitude;
        const json::String& title=interpreter["title"];
        m_title=std::string(title);
        const json::String& description=interpreter["description"];
        m_description=std::string(description);
        const json::String& time=interpreter["time"];
        m_time=std::string(time);
        const json::String& link=interpreter["link"];
        m_link=std::string(link);
}

void ApplyMarkJsonQuery::process(){
  CHandlePtr<std::vector<CHandlePtr<common::User> > > users=common::DbSession::getInstance().getUsers();
  m_status="error";
  for (std::vector<CHandlePtr<common::User> >::iterator i=users->begin();i!=users->end();i++)
  {
    if ((*i).dynamicCast<loader::User>()->getToken()==m_token)
    {
    	struct tm tim;	
        // syslog(LOG_INFO,"Find user from request");
        // syslog(LOG_INFO,"strptime result %i",strptime(m_time.c_str(),"%d %b %Y %H:%M:%S",&tim)==NULL);
        // syslog(LOG_INFO,"Finishing strptime");
    	CHandlePtr<common::Channels> channels = common::DbSession::getInstance().getChannels();
        // syslog(LOG_INFO,"getChannels sucsesfull");
    	for (common::Channels::iterator j=channels->begin();j!=channels->end();j++)
      {
    		if ((*j)->getName()==m_channel)
        {
                        // syslog(LOG_INFO,"Find channel from request");
			CHandlePtr<loader::DataMark> mark=makeHandle(new loader::DataMark(0,m_latitude,m_longitude,m_title,m_description,m_link,CTime(tim) ,*i,*j));
                        // syslog(LOG_INFO,"Datamark created sucsefully. Id of user %i, Id of channel %i, mark %i",(*i).dynamicCast<loader::User>()->getId(),(*j).dynamicCast<loader::Channel>()->getId(),mark==0);
          	//	try
		//	{
				common::DbSession::getInstance().updateChannel((*j).dynamicCast<loader::Channel>()->getId(),mark); // \ToDo replace channel_id to Objejct
		//	} catch (//const CDynamicCastFailure &x)
		///	{
				// type your code here
		//		break;
		//	}
          		(*j)->addData(mark);
                        // syslog(LOG_INFO,"updateChannel finished sucsesfull");
    			m_status="ok";
    			break;
    		}
	    }
	    break;
    }
  }
}

std::string ApplyMarkJsonQuery::outToString() const
{
	return "{\"status\":\""+m_status+"\"}";
}

/* ===[ End of file $HeadURL$ ]=== */