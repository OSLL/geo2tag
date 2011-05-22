#include <QSettings>
#include <QDebug>
#include "defines.h"

QString getServerUrl()
{
  QSettings settings(QSettings::SystemScope,"osll","libs");

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

  int serverPort = settings.value("server_port").toInt();

  if( serverPort == 0 )
  {
    serverPort = DEFAULT_PORT;
    setServerPort(serverPort);
  }

  return serverPort;
}


void setServerPort(int port)
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  settings.setValue("server_port",port);
}
