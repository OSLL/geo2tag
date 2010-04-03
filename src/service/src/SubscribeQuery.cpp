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
#include <syslog.h>
SubscribeQuery::SubscribeQuery(const std::stringstream& query){
  json::Element elemRoot;
  std::istringstream s(query.str());
  json::Reader::Read(elemRoot,s);
  json::QuickInterpreter interpreter(elemRoot);
  const json::String& user=interpreter["user"];
  m_user=std::string(user);
  const json::String& channel=interpreter["channel"];
  m_channel=std::string(channel);
}

void SubscribeQuery::subscribe(){
//  common::DbSession::getInstance().loadData();
  syslog(LOG_INFO,"Starting DbSession::subscribe method");
  common::DbSession::getInstance().subscribe(m_user,m_channel);
}

std::string SubscribeQuery::outToString() const
{
        syslog(LOG_INFO,"Starting SubscribedQuery::outToString() method");
        return "{\"status\":\"ok\"}";
}



/* ===[ End of file ]=== */
