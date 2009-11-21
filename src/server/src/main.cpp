#include <syslog.h>
#include <iostream>
#include "server.h"

int main(int argc, char ** argv) {
   openlog("/tmp/g2t_fcgi.log", LOG_CONS | LOG_NDELAY, LOG_USER);
    
    Server s;
    s.serve();
    
    return 0;
}
