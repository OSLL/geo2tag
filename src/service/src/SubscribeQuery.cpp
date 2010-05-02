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
SubscribeQuery::SubscribeQuery(){

}

void SubscribeQuery::init(const std::stringstream& query){

  json::Element elemRoot;
  std::istringstream s(query.str());
  json::Reader::Read(elemRoot,s);
  json::QuickInterpreter interpreter(elemRoot);
  const json::String& token=interpreter["auth_token"];
  m_token=std::string(token);
  const json::String& channel=interpreter["channel"];
  m_channel=std::string(channel);
}

void SubscribeQuery::process(){
//  common::DbSession::getInstance().loadData();
  syslog(LOG_INFO,"Starting DbSession::subscribe method");
  CHandlePtr<std::vector<CHandlePtr<common::User> > > users=common::DbSession::getInstance().getUsers();
  for (std::vector<CHandlePtr<common::User> >::iterator i=users->begin();i!=users->end();i++)
  {
    if ((*i).dynamicCast<loader::User>()->getToken()==m_token)
    {
      common::DbSession::getInstance().subscribe((*i)->getLogin(),m_channel);
    }
  }
}

std::string SubscribeQuery::outToString() const
{
        syslog(LOG_INFO,"Starting SubscribedQuery::outToString() method");
        return "{\"status\":\"ok\"}";
}



/* ===[ End of file ]=== */
