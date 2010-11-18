#include <QCoreApplication>
#include <syslog.h>
#include <iostream>
#include "server.h"
#include "DbSession.h"
#include "UpdateThread.h"
#include "UserInternal.h"
#include "ChannelInternal.h"

int main(int argc, char ** argv)
{
    QCoreApplication app(argc,argv);
    openlog("geo2tag_fcgi_test", LOG_CONS | LOG_NDELAY, LOG_USER);
    syslog(LOG_INFO, "starting...");

//    DbSession::getInstance();

//    Server s;
//    s.serve();
    Users *u = new Users();
    DataMarks *m = new DataMarks();
    Channels *c = new Channels();
    UpdateThread thr(QSharedPointer<DataMarks>(m),
                     QSharedPointer<Users>(u),
                     QSharedPointer<Channels>(c),
                     NULL);
    thr.start();


    return app.exec();
}
