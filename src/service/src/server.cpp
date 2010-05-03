#include <sstream>
#include <syslog.h>
#include <string.h>
#include <fcgi_stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "server.h"
#include "AvailableListJsonQuery.h"
#include "SubscribedListJsonQuery.h"
#include "SubscribeQuery.h"
#include "ApplyMarkJsonQuery.h"
#include "ApplyChannelJsonQuery.h"
#include "RssFeedJsonQuery.h"
#include "UnknownJsonQuery.h"
#include "UnsubscribeJsonQuery.h"
#include "LoginJsonQuery.h"
#include "DynamicCastFailure.h"

#define LISTENSOCK_FILENO 0
#define LISTENSOCK_FLAGS 0

Server::Server() 
{
    int err = FCGX_Init(); // call before Accept in multithreaded apps

    m_factory.reg<AvailableList>(AVAILABLE_LIST);
    m_factory.reg<SubscribedList>(SUBSCRIBED_LIST);
    m_factory.reg<SubscribeQuery>(SUBSCRIBE);
    m_factory.reg<ApplyMarkJsonQuery>(APPLYMARK);
    m_factory.reg<ApplyChannelJsonQuery>(APPLYCHANNEL);
    m_factory.reg<RssFeedJsonQuery>(RSSFEED);
    m_factory.reg<UnsubscribeJsonQuery>(UNSUBSCRIBE);
    m_factory.reg<LoginJsonQuery>(LOGIN);
    m_factory.reg<UnknownJsonQuery>(UNKNOWN);
    
    if (err) 
    {
        std::ostringstream s;
        s << "FCGX_Init failed: " << err;
        syslog(LOG_INFO, s.str().c_str());
    }

    err = FCGX_InitRequest(&m_cgi,LISTENSOCK_FILENO, LISTENSOCK_FLAGS);
    if (err) 
    {
        std::ostringstream s;
        s << "FCGX_InitRequest failed: " << err;
        syslog(LOG_INFO, s.str().c_str());
    }
}


void Server::process(const Query &q) 
{
    /*
     * We are a FastCGI module so we just need write our result to the standart output.
     */
	Stream s(m_cgi.out);

	std::string str=q.getStream().str();
  std::stringstream ss(str);
	syslog(LOG_INFO,ss.str().c_str());

	CHandlePtr<IJsonQuery> query=m_factory.getJsonQuery(q,ss);
	try
  {
		query->process();
		s << "Status: 200 OK\r\nContent-Type: text/html\r\n\r\n";
		s << (*query);
	}
	catch (CDynamicCastFailure& e)
	{
		syslog(LOG_INFO,"Dynamic Cast Failure. Line %i, file %s",e.getLine(),e.getSource());
		s << "Status: 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n Error applying mark.";
	} 
}

void Server::serve() 
{
  syslog(LOG_INFO,"Starting serve");
  while (1) 
  {
    int err = FCGX_Accept_r(&m_cgi);
    if (err) 
    {
      syslog(LOG_INFO, "FCGX_Accept_r stopped: %d", err);
      break;
    }
    const std::string s=std::string(FCGX_GetParam("QUERY_STRING", m_cgi.envp));
    syslog(LOG_INFO,"Recieved new query. Content below");
    Stream stream=Stream(m_cgi.in);
    Query q(s, stream);
    syslog(LOG_INFO, q.getStream().str().c_str());
    process(q);
    syslog(LOG_INFO,"processing is finished");
    FCGX_Finish_r(&m_cgi);
  }
}

Server::~Server() 
{
    FCGX_ShutdownPending();
}
