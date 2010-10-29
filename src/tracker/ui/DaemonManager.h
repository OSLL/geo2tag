#ifndef DAEMONMANAGER_H
#define DAEMONMANAGER_H

#include <QString>
#include <QObject>

class DaemonManager
{
    QObject object;

public:
    DaemonManager();

    static DaemonManager& getInstance();
    void start();
    void stop();
    QString getStatus();
};

#endif // DAEMONMANAGER_H
