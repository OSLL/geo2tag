#include "DaemonManager.h"

#include <QProcess>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QFileInfo>

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


Status DaemonManager::getStatus()
{
  QFile log(LOG);
  if (!log.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qDebug() << "can't open file with status";
  }

  QTextStream in(&log);
  struct Status retStatus;
  retStatus.valid = false;
  QString input;
  QString datetime;
  QString status;
  QString description("");

  if (!in.atEnd())
    in >> input;
  else
    goto exit;

  datetime = input;

  if (!in.atEnd())
    in >> input;
  else
    goto exit;

  datetime = datetime + " " + input;

  if (!in.atEnd())
    in >> input;
  else
    goto exit;

  status = input;

  while(!in.atEnd())
  {
    in >> input;
    description = description + " " + input;
  }
  description = description.trimmed();

  retStatus.datetime = QDateTime::fromString(datetime, "hh:mm:ss dd.MM.yyyy");
  retStatus.status = status;
  retStatus.description = description;

  exit:
  log.close();
  return retStatus;
}


QDateTime DaemonManager::lastStatusModification()
{
  QFileInfo logInfo(LOG);
  return logInfo.lastModified();
}
