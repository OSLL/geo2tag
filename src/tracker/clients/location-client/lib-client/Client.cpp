#include "Client.h"
#include <QDebug>
#include <QEventLoop>
#include "JsonDataMark.h"
#include "JsonChannel.h"
#include "JsonUser.h"
#include "GpsInfo.h"

#include <QTimer>
#include <QVariant>

// Interval for different requests
#define PAUSE_INTERVAL 250
#define DEFAULT_CHANNEL "default"

Client::Client(QObject *parent) :
    QObject(parent)
{
    m_loginQuery = new LoginQuery(this);
    connect(m_loginQuery, SIGNAL(connected()), SLOT(onAuthentificated()));
    connect(m_loginQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));

    connect(m_loginQuery, SIGNAL(errorOccured(int)),SLOT(onError(int)));


    m_netManager = new QNetworkConfigurationManager(this);

}



void Client::auth(QString user, QString pass)
{
  m_loginQuery->setQuery(user,pass);
  m_loginQuery->doRequest();
}




void Client::onError(QString err)
{

    emit error(QVariant(err));
  m_lastError = err;
  if(!m_authentificated)
  {
    // Retry authentification
    qDebug() << "Error during authentification";
    pause(1000);
    m_loginQuery->doRequest();

  }
  else
  {
    qDebug() << "Error during mark sending";
  }
}

void Client::onError(int err)
{
    if (err==INCORRECT_CREDENTIALS_ERROR)
    {
        emit error(QVariant("Incorrect login or password"));

    }
}


bool Client::isAuthentificated()
{
  return m_authentificated;
}


QString Client::getLastError()
{
  return m_lastError;
}


// Return internete state
bool Client::isOnline()
{
  return m_netManager->isOnline();
}


void Client::onAuthentificated()
{
  qDebug() << "Authentificated " <<  m_loginQuery->getUser()->getToken();
  m_user =  m_loginQuery->getUser();
  m_authentificated = true;
  emit authentificated(QVariant(m_user->getLogin()));
}

void Client::pause(int msecs)
{
  QEventLoop eventLoop;
  QTimer::singleShot(msecs, &eventLoop, SLOT(quit()));
  eventLoop.exec();
}

