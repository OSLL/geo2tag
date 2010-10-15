/*! ---------------------------------------------------------------
 *  
 *
 * \file SubscribeQuery.cpp
 * \brief SubscribeQuery implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "SubscribeQuery.h"
#include "UserInternal.h"
#include <syslog.h>
SubscribeQuery::SubscribeQuery()
{
    m_status_description = "nothing";
}

void SubscribeQuery::init(const std::stringstream& query)
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

void SubscribeQuery::process()
{
//  common::DbSession::getInstance().loadData();
  // syslog(LOG_INFO,"Starting DbSession::subscribe method");
  CHandlePtr<common::Channels > channels=common::DbSession::getInstance().getChannels();
  CHandlePtr<common::User> du;
  CHandlePtr<common::Channel> ch;

  if(common::DbSession::getInstance().getTokensMap().count(m_token) == 0)
  {
    syslog(LOG_INFO,"common::DbSession::getInstance().getTokensMap().count(m_token) == 0");
    m_result="Error";
    m_status_description = "User isn't authenticated";
    return;
  } 
  else
  {
    du = common::DbSession::getInstance().getTokensMap().find(m_token)->second;  
  }

  for(size_t i=0; i< channels->size(); ++i)
  {
      if((*channels)[i]->getName() == m_channel)
      {
          ch = (*channels)[i];
          break;
      }
  }
  
  if(ch == NULL) // Channel was not found
  { 
    // syslog(LOG_INFO, "ch == NULL");
    m_result = "Error";
    m_status_description = "Channel wasn't found";
    return;
  }

  try 
  { 
    common::DbSession::getInstance().subscribe(du,ch);
    m_result = "Ok";
  }
  catch(const CExceptionSource& x)
  {
    syslog(LOG_INFO,"exception while common::DbSession::getInstance().subscribe(du,ch)");
    m_result = "Error";
    m_status_description = "Db internal error";
  }

}

std::string SubscribeQuery::outToString() const
{
        // syslog(LOG_INFO,"Starting SubscribedQuery::outToString() method");
        return "{\"status\":\"" + m_result +
                "\", \"status_description\":\"" + m_status_description+"\"}";
}



/* ===[ End of file ]=== */
