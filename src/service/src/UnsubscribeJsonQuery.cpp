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
 * \file UnsubscribeJsonQuery.cpp
 * \brief UnsubscribeJsonQuery implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "UnsubscribeJsonQuery.h"
#include "ChannelInternal.h"
#include "DataMarkInternal.h"
#include "UserInternal.h"
#include <time.h>
#include "DbSession.h"
#include "Time.h"
#include <syslog.h>

UnsubscribeJsonQuery::UnsubscribeJsonQuery()
{
	m_status="Error";
        m_status_description = "nothing";
}

void UnsubscribeJsonQuery::init(const std::stringstream& query)
{
	json::Element elemRoot;
	std::istringstream s(query.str());
	json::Reader::Read(elemRoot,s);
	json::QuickInterpreter interpreter(elemRoot);
	const json::String& token=interpreter["auth_token"];
	m_token=std::string(token);
	const json::String& channel=interpreter["channel"];
	m_channel=std::string(channel);
}

void UnsubscribeJsonQuery::process()
{
    CHandlePtr<std::vector<CHandlePtr<common::User> > > users=common::DbSession::getInstance().getUsers();
    m_status_description = "User isn't authenticated";
    for (std::vector<CHandlePtr<common::User> >::iterator i=users->begin();i!=users->end();i++)
    {
        if ((*i).dynamicCast<loader::User>()->getToken()==m_token)
        {
            m_status_description = "Channel wasn't found";
            CHandlePtr<common::Channels> channels=(*i)->getSubscribedChannels();
            if(channels == NULL)
            {
                break;
            }
            for (common::Channels::iterator j=channels->begin();j!=channels->end();j++)
            {
                if ((*j)->getName()==m_channel)
                {
                    common::DbSession::getInstance().unsubscribe((*i),(*j));
                    // syslog(LOG_INFO,"DbSession::Unsubscribe finished");
                    m_status="Ok";
                    m_status_description = "nothing";
                    return;
                }
            }
            break;
        }
    }
    m_status="Error";
}

std::string UnsubscribeJsonQuery::outToString() const
{
    return "{\"status\":\"" + m_status +
            "\", \"status_description\":\"" + m_status_description+"\"}";
}

/* ===[ End of file $HeadURL$ ]=== */
