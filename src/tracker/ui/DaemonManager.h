#ifndef DAEMONMANAGER_H
#define DAEMONMANAGER_H

#include <QString>

class DaemonManager
{
public:
    DaemonManager();

    static DaemonManager& getInstance();
    void start();
    void stop();
    QString getStatus();
};

#endif // DAEMONMANAGER_H
