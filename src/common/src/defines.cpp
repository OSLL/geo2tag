#include <QSettings>
#include <QDebug>
#include "defines.h"

QString getServerUrl()
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  if (settings.value("server_url").toString().isEmpty())
  {
    return DEFAULT_SERVER;
  }

  QString serverUrl=settings.value("server_url").toString();

  if(serverUrl == "")
  {
    serverUrl = DEFAULT_SERVER;
    setServerUrl(serverUrl);
  }

  return serverUrl;
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
  int serverPort = DEFAULT_PORT;
  setServerPort(serverPort);

  return serverPort;
}


void setServerPort(int port)
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  settings.setValue("server_port",port);
}
