#include <QDebug>

#include "AddUserQuery.h"
#include "defines.h"

#include "JsonUser.h"
#include "AddUserRequestJSON.h"
#include "AddUserResponseJSON.h"

#ifndef Q_OS_SYMBIAN
#include <syslog.h>
#else
#include "symbian.h"
#endif

AddUserQuery::AddUserQuery(const QString &login, const QString &password, QObject *parent):
DefaultQuery(parent), m_login(login), m_password(password)
{
}


AddUserQuery::AddUserQuery(QObject *parent):
DefaultQuery(parent)
{
}


QString AddUserQuery::getUrl() const
{
  return ADD_USER_HTTP_URL;
}


QByteArray AddUserQuery::getRequestBody() const
{
  QSharedPointer<common::User> dummyUser(new JsonUser(m_login,m_password));
  AddUserRequestJSON request;
  request.addUser(dummyUser);
  return request.getJson();
}


void AddUserQuery::processReply(QNetworkReply *reply)
{
  #ifndef Q_OS_SYMBIAN
  AddUserResponseJSON response;
  response.parseJson(reply->readAll());
  m_errno = response.getErrno();
  if(response.getErrno() == SUCCESS)
  {
    QSharedPointer<common::User> user = response.getUsers()->at(0);
    m_user = QSharedPointer<common::User>(new JsonUser(m_login, m_password, user->getToken()));
    syslog(LOG_INFO,"!!connected!");
    Q_EMIT connected();
  }
  else
  {
    Q_EMIT errorOccured(response.getErrno());
  }
  #endif
}


void AddUserQuery::setQuery(const QString& login, const QString& password)
{
  m_login=login;
  m_password=password;
}


QSharedPointer<common::User> AddUserQuery::getUser() const
{
  return m_user;
}


AddUserQuery::~AddUserQuery()
{

}
