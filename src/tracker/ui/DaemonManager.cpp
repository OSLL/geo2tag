#include "DaemonManager.h"

#include <QString>

DaemonManager::DaemonManager()
{
}

DaemonManager& DaemonManager::getInstance()
{
    static DaemonManager dM;
    return dM;
}

void DaemonManager::start()
{
    // TODO
}

void DaemonManager::stop()
{
    // TODO
}

QString DaemonManager::getStatus()
{
    // TODO
    return QString("undefined");
}


