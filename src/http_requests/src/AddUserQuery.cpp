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

AddUserQuery::AddUserQuery(const QString &login, const QString &password, QObject *parent, const QString& email)
: DefaultQuery(parent),
m_login(login),
m_password(password),
m_email(email)
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
  QSharedPointer<common::User> dummyUser(new JsonUser(m_login, m_password, m_email));
  AddUserRequestJSON request;
  request.addUser(dummyUser);
  return request.getJson();
}


void AddUserQuery::processResponse(const QByteArray &data)
{
  #ifndef Q_OS_SYMBIAN
  AddUserResponseJSON response;
  response.parseJson(data);

  m_session = response.getSessions()->at(0);
  m_user = QSharedPointer<common::User>(new JsonUser(m_login, m_password, m_email));
  #endif
}


void AddUserQuery::setQuery(const QString& login, const QString& password)
{
  m_login=login;
  m_password=password;
}


void AddUserQuery::setQuery(const QString& login, const QString& password, const QString& email)
{
  m_login = login;
  m_password = password;
  m_email = email;
}


QSharedPointer<common::User> AddUserQuery::getUser() const
{
  return m_user;
}


QSharedPointer<Session> AddUserQuery::getSession() const
{
  return m_session;
}


AddUserQuery::~AddUserQuery()
{

}
