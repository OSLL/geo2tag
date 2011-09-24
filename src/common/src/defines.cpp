#include <QSettings>
#include <QDebug>
#include <syslog.h>
#include "defines.h"

QString getServerUrl()
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  if (settings.value("server_url").toString().isEmpty())
  {
    return DEFAULT_SERVER;
  }
  return settings.value("server_url").toString();

}


void setServerUrl(QString serverUrl)
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  settings.setValue("server_url",serverUrl);
}


int getServerPort()
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  if (settings.value("server_port").toInt()==0)
  {
    return DEFAULT_PORT;
  }
  return settings.value("server_port").toInt();
}


void setServerPort(int port)
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  settings.setValue("server_port",port);
}
