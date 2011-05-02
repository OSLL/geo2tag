#ifndef DAEMONMANAGER_H
#define DAEMONMANAGER_H

#include <QString>
#include <QObject>
#include <QFile>

#include "Status.h"

#define LOG QString("/var/wikigps-tracker")

class DaemonManager
{
  QObject object;
  QFile *m_log;

  public:
    DaemonManager();

    static DaemonManager& getInstance();
    void start();
    void stop();
    Status getStatus();
    QDateTime lastStatusModification();
    };
#endif                                                      // DAEMONMANAGER_H
