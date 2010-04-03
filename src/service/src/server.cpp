#include <iostream>
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
#include "RssFeedJsonQuery.h"
//#include "engine.h"

#define LISTENSOCK_FILENO 0
#define LISTENSOCK_FLAGS 0

Server::Server() {
    int err = FCGX_Init(); // call before Accept in multithreaded apps
    if (err) {
        std::ostringstream s;
        s << "FCGX_Init failed: " << err;
        syslog(LOG_INFO, s.str().c_str());
    }

    err = FCGX_InitRequest(&m_cgi,LISTENSOCK_FILENO, LISTENSOCK_FLAGS);
    if (err) {
        std::ostringstream s;
        s << "FCGX_InitRequest failed: " << err;
        syslog(LOG_INFO, s.str().c_str());
    }
}

void Server::process(const Query &q) {
    /*
     * We are a FastCGI module so we just need write our result to the standart output.
     */
	Stream s(m_cgi.out);
	switch (q.getType()){
		case AVAILABLE_LIST:
    {
		        syslog(LOG_INFO,"AvailableList query recieved");
			std::string str=q.getStream().str();
                        std::stringstream ss(str);
			syslog(LOG_INFO,"This string will putted into AvailableList");
			syslog(LOG_INFO,ss.str().c_str());
	                AvailableList list(ss);
			syslog(LOG_INFO,"AvailableList created");
			list.process();
                        s << "Status: 200 OK\r\nContent-Type: text/html\r\n\r\n";
			s << list;
			break;
		}
		case SUBSCRIBED_LIST:
		{
			syslog(LOG_INFO,"SubscribedList query recieved");
			std::string str=q.getStream().str();
                        std::stringstream ss(str);
                        syslog(LOG_INFO,"This string will putted into SubscribedList");
                        syslog(LOG_INFO,ss.str().c_str());
                        SubscribedList list(ss);
                        syslog(LOG_INFO,"SubscribedList created");
                        list.process();
			s << "Status: 200 OK\r\nContent-Type: text/html\r\n\r\n";
                        s << list;
                        break;
		}
                case SUBSCRIBE:
                {
                        syslog(LOG_INFO,"Subscribe query recieved");
                        std::string str=q.getStream().str();
                        std::stringstream ss(str);
                        syslog(LOG_INFO,"This string will putted into SubscribeQuery");
                        syslog(LOG_INFO,ss.str().c_str());
                        SubscribeQuery list(ss);
                        syslog(LOG_INFO,"SibscribeQuery created");
                        list.subscribe();
                        s << "Status: 200 OK\r\nContent-Type: text/html\r\n\r\n";
                        s << list;
                        break;
                }
		case APPLYMARK:
		{
                        syslog(LOG_INFO,"ApplyMark query recieved");
                        std::string str=q.getStream().str();
                        std::stringstream ss(str);
                        syslog(LOG_INFO,"This string will putted into ApplyMark");
                        syslog(LOG_INFO,ss.str().c_str());
                        ApplyMarkJsonQuery list(ss);
                        syslog(LOG_INFO,"ApplyMarkQuery created");
                        list.process();
                        s << "Status: 200 OK\r\nContent-Type: text/html\r\n\r\n";
                        s << list;
                        break;

		}
                case RSSFEED:
                {
                        syslog(LOG_INFO,"RssFeed query recieved");
                        std::string str=q.getStream().str();
                        std::stringstream ss(str);
                        syslog(LOG_INFO,"This string will putted into RssFeed");
                        syslog(LOG_INFO,ss.str().c_str());
                        RssFeedJsonQuery list(ss);
                        syslog(LOG_INFO,"RssFeedQuery created");
                        list.process();
                        s << "Status: 200 OK\r\nContent-Type: text/html\r\n\r\n";
                        s << list;
                        break;

                }

		default :{
			s << "Status: 200 OK\r\nContent-Type: text/html\r\n\r\n";
			s << "Unknown query";

	}
}
}

void Server::serve() {
    syslog(LOG_INFO,"Starting serve");
    while (1) {
        int err = FCGX_Accept_r(&m_cgi);
        if (err) {
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

Server::~Server() {
    FCGX_ShutdownPending();
}
