#ifndef _SERVER_H_
#define _SERVER_H_

#include <fcgiapp.h>

class Server {

    FCGX_Request m_cgi;

    void process(const Query& q);

public:

    Server();

    void serve();

    ~Server();
};

#endif
