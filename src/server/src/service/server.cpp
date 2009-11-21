#include <iostream>
#include <sstream>
#include <syslog.h>
#include <string.h>

#include "server.h"
#include "engine.h"

#define LISTENSOCK_FILENO 0
#define LISTENSOCK_FLAGS 0

Server::Server() {
    int err = FCGX_Init(); // call before Accept in multithreaded apps
    if (err) {
        std::ostringstream s;
        s << "FCGX_Init failed: " << err;
        syslog(LOG_INFO, s.str().c_str());
    }

    err = FCGX_InitRequest(&m_cgi, LISTENSOCK_FILENO, LISTENSOCK_FLAGS);
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
    s << Engine::getInstance().process(q.getType(), q.getStream());
}

void Server::serve() {
    while (1) {
        int err = FCGX_Accept_r(&m_cgi);
        if (err) {
            syslog(LOG_INFO, "FCGX_Accept_r stopped: %d", err);
            break;
        }

        Query q(FCGX_GetParam("QUERY_STRING", m_cgi.envp), Stream(m_cgi.in));
        process(q);

        FCGX_Finish_r(&m_cgi);
    }
}

Server::~Server() {
    FCGX_ShutdownPending();
}
