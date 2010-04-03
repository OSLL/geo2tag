#include <syslog.h>
#include <iostream>
#include "server.h"
#include "DbSession.h"

int main(int argc, char ** argv) {
    openlog("geo2tag_fcgi_test", LOG_CONS | LOG_NDELAY, LOG_USER);
    syslog(LOG_INFO, "starting...");
    common::DbSession::getInstance();
    Server s;
    s.serve();   
    return 0;
}
