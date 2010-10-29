#include "DaemonManager.h"

#include <QProcess>
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
    QString program = "/etc/init.d/tracker";
    QStringList arguments;
    arguments << "start";

    QProcess *myProcess = new QProcess(&object);
    myProcess->start(program, arguments);
}

void DaemonManager::stop()
{
    QString program = "/etc/init.d/tracker";
    QStringList arguments;
    arguments << "stop";

    QProcess *myProcess = new QProcess(&object);
    myProcess->start(program, arguments);
}

QString DaemonManager::getStatus()
{
    // TODO
    return QString("undefined");
}


