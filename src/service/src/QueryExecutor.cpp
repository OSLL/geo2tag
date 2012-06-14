/*
 * Copyright 2010-2011  OSLL osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
/*----------------------------------------------------------------- !
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include <syslog.h>
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QString>

#include "QueryExecutor.h"
#include "JsonUser.h"
#include "DataMarkInternal.h"
#include "UserInternal.h"
#include "ChannelInternal.h"
#include "SessionInternal.h"
#include "PerformanceCounter.h"
 

QueryExecutor::QueryExecutor(const QSqlDatabase &db, QObject *parent): QObject(parent),
m_database(db)
{
}


bool QueryExecutor::connect()
{
  return m_database.open();
}


bool QueryExecutor::isConnected()
{
  return m_database.isOpen();
}


qlonglong QueryExecutor::nextKey(const QString &sequence) const
{
  QSqlQuery query(m_database);
  query.exec("select nextval('"+sequence+"') as seq;");
  qlonglong id;
  if ( query.next() )
    id = query.value(0).toLongLong();
  return id;
}


qlonglong QueryExecutor::nextTagKey() const
{
  return nextKey("tags_seq");
}


qlonglong QueryExecutor::nextUserKey() const
{
  return nextKey("users_seq");
}


qlonglong QueryExecutor::nextChannelKey() const
{
  return nextKey("channels_seq");
}



qlonglong QueryExecutor::nextSessionKey() const
{
  return nextKey("sessions_seq");
}

const QString QueryExecutor::generateNewToken(const QString& login,const QString& password) const
{
  QString log=login+password;
  QByteArray toHash(log.toUtf8());
  toHash=QCryptographicHash::hash(log.toUtf8(),QCryptographicHash::Md5);
  QString result(toHash.toHex());
  syslog(LOG_INFO,"TOken = %s",result.toStdString().c_str());
  return result;
}

const QString QueryExecutor::generateNewToken(const QString& email, const QString& login,const QString& password) const
{
  QString log=login+password+email;
  QByteArray toHash(log.toUtf8());
  toHash=QCryptographicHash::hash(log.toUtf8(),QCryptographicHash::Md5);
  QString result(toHash.toHex());
  syslog(LOG_INFO,"TOken = %s",result.toStdString().c_str());
  return result;
}

const QString QueryExecutor::generateNewToken(const QString& accessTime, const QString& email, const QString& login,const QString& password) const
{
  QString log=login+password+email+accessTime;
  QByteArray toHash(log.toUtf8());
  toHash=QCryptographicHash::hash(log.toUtf8(),QCryptographicHash::Md5);
  QString result(toHash.toHex());
  syslog(LOG_INFO,"TOken = %s",result.toStdString().c_str());
  return result;
}


QSharedPointer<DataMark> QueryExecutor::insertNewTag(const QSharedPointer<DataMark>& tag)
{
  PerformanceCounter counter("QueryExecutor::insertNewTag");
  bool result;
  qlonglong newId = nextTagKey();

  syslog(LOG_INFO, "%s", QString("insertNewTag-start-").append(QString::number(newId)).toStdString().c_str());

  QSqlQuery newTagQuery(m_database);
  newTagQuery.prepare("insert into tag (altitude , latitude, longitude, label, description, url, user_id, time, id, channel_id) "
    "         values(:altitude,:latitude,:longitude,:label,:description,:url,:user_id,:time,:id, :channel_id);");
  newTagQuery.bindValue(":altitude", tag->getAltitude());
  newTagQuery.bindValue(":latitude", tag->getLatitude());
  newTagQuery.bindValue(":longitude", tag->getLongitude());
  newTagQuery.bindValue(":label", tag->getLabel().isEmpty()? "New Mark" : tag->getLabel());
  newTagQuery.bindValue(":description", tag->getDescription());
  newTagQuery.bindValue(":url", tag->getUrl());
  newTagQuery.bindValue(":user_id", tag->getUser()->getId());
  newTagQuery.bindValue(":channel_id", tag->getChannel()->getId());
  newTagQuery.bindValue(":time", tag->getTime().toUTC());
  newTagQuery.bindValue(":id", newId);


  m_database.transaction();

  result = newTagQuery.exec();
  if(!result)
  {
    m_database.rollback();
    syslog(LOG_INFO, "Rollback for NewTag sql query");
    return QSharedPointer<DataMark>(NULL);
  }

  m_database.commit();

  QSharedPointer<DataMark> t(
    new DbDataMark(newId, tag->getAltitude(), tag->getLatitude(),
    tag->getLongitude(), tag->getLabel(),
    tag->getDescription(), tag->getUrl(),
    tag->getTime(), tag->getUser()->getId(),tag->getChannel()->getId()));
  t->setUser(tag->getUser());
  t->setChannel(tag->getChannel());

  return t;
}


QSharedPointer<Channel> QueryExecutor::insertNewChannel(const QSharedPointer<Channel>& channel)
{
  PerformanceCounter counter("QueryExecutor::insertNewChannel");
  bool result;
  QSqlQuery newChannelQuery(m_database);
  qlonglong newId = nextChannelKey();
  syslog(LOG_INFO,"NewId ready, now preparing sql query for adding new channel");
  newChannelQuery.prepare("insert into channel (id,name,description,url) values(:id,:name,:description,:url);");
  newChannelQuery.bindValue(":id",newId);
  newChannelQuery.bindValue(":name",channel->getName());
  newChannelQuery.bindValue(":description",channel->getDescription());
  newChannelQuery.bindValue(":url",channel->getUrl());

  m_database.transaction();

  result=newChannelQuery.exec();
  if(!result)
  {
    syslog(LOG_INFO,"Rollback for NewChannel sql query");
    m_database.rollback();
    return QSharedPointer<Channel>(NULL);
  }else
  syslog(LOG_INFO,"Commit for NewChannel sql query - insert in table channel");

  m_database.commit();

  QSharedPointer<DbChannel> newChannel(new DbChannel(newId,channel->getName(),channel->getDescription(),channel->getUrl()));
  return newChannel;
}

bool QueryExecutor::doesTmpUserExist(const QSharedPointer<common::User> &user)
{
    PerformanceCounter counter("QueryExecutor::doesTmpUserExist");
    QSqlQuery query(m_database);
    query.prepare("select * from signups where login = :login or email = :email;");
    query.bindValue(":login",user->getLogin());
    query.bindValue(":email",user->getEmail());
    syslog(LOG_INFO,"Selecting: %s", query.lastQuery().toStdString().c_str());

    query.exec();

    if (query.next()) {
        return true;
    } else {
        syslog(LOG_INFO,"No matching users.");
        return false;
    }
}

bool QueryExecutor::doesUserWithGivenEmailExist(const QSharedPointer<common::User> &user)
{
    PerformanceCounter counter("QueryExecutor::doesUserWithGivenEmailExist");
    QSqlQuery query(m_database);
    syslog(LOG_INFO, "Checking of user existence in users by email: %s", user->getEmail().toStdString().c_str());

    query.prepare("select id from users where email = :email;");
    query.bindValue(":email", user->getEmail());
    syslog(LOG_INFO,"Selecting: %s", query.lastQuery().toStdString().c_str());
    query.exec();

    if (query.next()) {
        syslog(LOG_INFO,"Match found.");
        return true;
    } else {
        syslog(LOG_INFO,"No matching users.");
        return false;
    }
}

bool QueryExecutor::deleteTmpUser(const QSharedPointer<common::User> &user)
{
    PerformanceCounter counter("QueryExecutor::deleteTmpUser"); 
    bool result;
    QSqlQuery deleteSignupQuery(m_database);
    deleteSignupQuery.prepare("delete from signups where login = :login;");
    deleteSignupQuery.bindValue(":login",user->getLogin() );
    syslog(LOG_INFO,"Deleting: %s", deleteSignupQuery.lastQuery().toStdString().c_str());

    m_database.transaction();
    result = deleteSignupQuery.exec();
    if(!result) {
      syslog(LOG_INFO,"Rollback for deleteSignup sql query");
      m_database.rollback();
    } else {
      syslog(LOG_INFO,"Commit for deleteSignup sql query");
      m_database.commit();
    }
    return result;
}

const QString QueryExecutor::insertNewTmpUser(const QSharedPointer<common::User> &user)
{
    PerformanceCounter counter("QueryExecutor::insertNewTmpUser");
    bool result;
    QSqlQuery newSignupQuery(m_database);
    qlonglong newId = nextUserKey();
    syslog(LOG_INFO,"Generating token for new signup, %s : %s",user->getLogin().toStdString().c_str()
                                                            ,user->getPassword().toStdString().c_str());
    QString newToken = generateNewToken(user->getEmail(), user->getLogin(),user->getPassword());
    newSignupQuery.prepare("insert into signups (id,email,login,password,registration_token,sent) values(:id,:email,:login,:password,:r_token,:sent);");
    newSignupQuery.bindValue(":id", newId);
    newSignupQuery.bindValue(":email", user->getEmail());
    newSignupQuery.bindValue(":login", user->getLogin());
    newSignupQuery.bindValue(":password", user->getPassword());
    newSignupQuery.bindValue(":r_token", newToken);
    newSignupQuery.bindValue(":sent", FALSE);

    m_database.transaction();
    result = newSignupQuery.exec();
    if(!result) {
      syslog(LOG_INFO,"Rollback for NewSignup sql query");
      m_database.rollback();
      return QString("");
    } 
    syslog(LOG_INFO,"Commit for NewSignup sql query");
    m_database.commit();

    return newToken;
}

bool QueryExecutor::doesRegistrationTokenExist(const QString &token)
{
    PerformanceCounter counter("QueryExecutor::doesRegistrationTokenExist"); 
    QSqlQuery query(m_database);
    syslog(LOG_INFO, "Checking of user existence in signups by token: %s", token.toStdString().c_str());

    query.prepare("select id from signups where registration_token = :token;");
    query.bindValue(":token", token);
    syslog(LOG_INFO,"Selecting: %s", query.lastQuery().toStdString().c_str());
    query.exec();

    if (query.next()) {
        syslog(LOG_INFO,"Match found.");
        return true;
    } else {
        syslog(LOG_INFO,"No matching users.");
        return false;
    }
}

QSharedPointer<common::User> QueryExecutor::insertTmpUserIntoUsers(const QString &token)
{
    PerformanceCounter counter("QueryExecutor::insertTmpUserIntoUsers");
    QSqlQuery checkQuery(m_database);
    syslog(LOG_INFO, "Checking of user existence in signups by token: %s", token.toStdString().c_str());

    checkQuery.prepare("select email, login, password from signups where registration_token = :token;");
    checkQuery.bindValue(":token", token);
    syslog(LOG_INFO,"Selecting: %s", checkQuery.lastQuery().toStdString().c_str());
    checkQuery.exec();

    if (checkQuery.next()) {
        syslog(LOG_INFO,"Match found.");
        QString email = checkQuery.value(0).toString();
        QString login = checkQuery.value(1).toString();
        QString password = checkQuery.value(2).toString();
        const QSharedPointer<common::User> newUser(new common::User(login, password, email));
        QSharedPointer<common::User> insertedUser = insertNewUser(newUser);
        return insertedUser;
    } else {
        syslog(LOG_INFO,"No matching users.");
        return QSharedPointer<common::User>(NULL);;
    }
}

bool QueryExecutor::deleteTmpUser(const QString &token)
{
    PerformanceCounter counter("QueryExecutor::deleteTmpUser");
    bool result;
    QSqlQuery deleteSignupQuery(m_database);
    deleteSignupQuery.prepare("delete from signups where registration_token = :token;");
    deleteSignupQuery.bindValue(":token", token);
    syslog(LOG_INFO,"Deleting: %s", deleteSignupQuery.lastQuery().toStdString().c_str());

    m_database.transaction();
    result = deleteSignupQuery.exec();
    if(!result) {
      syslog(LOG_INFO,"Rollback for deleteSignup sql query");
      m_database.rollback();
    } else {
      syslog(LOG_INFO,"Commit for deleteSignup sql query");
      m_database.commit();
    }
    return result;
}

QSharedPointer<common::User> QueryExecutor::insertNewUser(const QSharedPointer<common::User>& user)
{
  PerformanceCounter counter("QueryExecutor::insertNewUser");
  bool result;
  QSqlQuery newUserQuery(m_database);
  qlonglong newId = nextUserKey();
  syslog(LOG_INFO,"Generating token for new user, %s : %s",user->getLogin().toStdString().c_str()
    ,user->getPassword().toStdString().c_str());
  QString newToken = generateNewToken(user->getLogin(),user->getPassword());
  //  syslog(LOG_INFO,"newToken = %s",newToken.toStdString().c_str());
  newUserQuery.prepare("insert into users (id,email,login,password) values(:id,:email,:login,:password);");
  newUserQuery.bindValue(":id",newId);
  newUserQuery.bindValue(":email",user->getEmail());
  newUserQuery.bindValue(":login",user->getLogin());
  newUserQuery.bindValue(":password",user->getPassword());
  m_database.transaction();
  result=newUserQuery.exec();

  QSharedPointer<common::User> newUser = QSharedPointer<common::User>(NULL);
  if(!result)
  {
    syslog(LOG_INFO,"Rollback for NewUser sql query");
    m_database.rollback();
  }else
  {
    syslog(LOG_INFO,"Commit for NewUser sql query");
    newUser = QSharedPointer<common::User>(new DbUser(user->getLogin(),user->getPassword(),newId));
    m_database.commit();
  }
  return newUser;
}


bool QueryExecutor::subscribeChannel(const QSharedPointer<common::User>& user,const QSharedPointer<Channel>& channel)
{
  PerformanceCounter counter("QueryExecutor::subscribeChannel"); 
  bool result;
  QSqlQuery insertNewSubscribtion(m_database);
  insertNewSubscribtion.prepare("insert into subscribe (channel_id,user_id) values(:channel_id,:user_id);");
  insertNewSubscribtion.bindValue(":channel_id",channel->getId());
  insertNewSubscribtion.bindValue(":user_id",user->getId());
  syslog(LOG_INFO,"Subscribing %s (Id = %lld) for %s (Id = %lld)",user->getLogin().toStdString().c_str(),user->getId(),
    channel->getName().toStdString().c_str(),channel->getId());

  m_database.transaction();
  result=insertNewSubscribtion.exec();
  if(!result)
  {
    syslog(LOG_INFO,"Rollback for subscribeChannel sql query");
    m_database.rollback();
  }else
  {
    syslog(LOG_INFO,"Commit for subscribeChannel sql query");
    m_database.commit();
  }
  return result;
}

bool QueryExecutor::unsubscribeChannel(const QSharedPointer<common::User>& user,const QSharedPointer<Channel>& channel)
{
  PerformanceCounter counter("QueryExecutor::unsubscribeChannel"); 
  bool result;
  QSqlQuery deleteSubscribtion(m_database);
  deleteSubscribtion.prepare("delete from subscribe where channel_id = :channel_id AND user_id = :user_id;");
  deleteSubscribtion.bindValue(":channel_id",channel->getId());
  deleteSubscribtion.bindValue(":user_id",user->getId());
  syslog(LOG_INFO,"Unsubscribing %s (Id = %lld) for %s (Id = %lld)",user->getLogin().toStdString().c_str(),user->getId(),
    channel->getName().toStdString().c_str(),channel->getId());

  m_database.transaction();
  result=deleteSubscribtion.exec();
  if(!result)
  {
    syslog(LOG_INFO,"Rollback for unsubscribeChannel sql query");
    m_database.rollback();
  }else
  {
    syslog(LOG_INFO,"Commit for unsubscribeChannel sql query");
    m_database.commit();
  }
  return result;
}


bool QueryExecutor::deleteUser(const QSharedPointer<common::User> &user)
{
    PerformanceCounter counter("QueryExecutor::deleteUser");
    bool result;
    QSqlQuery deleteUserQuery(m_database);
    syslog(LOG_INFO,"Deleting: id = %lld", user->getId());
    deleteUserQuery.prepare("delete from users where id = :id;");
    deleteUserQuery.bindValue(":id",user->getId() );

    m_database.transaction();
    result = deleteUserQuery.exec();
    if(!result) {
      syslog(LOG_INFO,"Rollback for deleteUser sql query");
      m_database.rollback();
    } else {
      syslog(LOG_INFO,"Commit for deleteUser sql query");
      m_database.commit();
    }
    return result;
}

QSharedPointer<Session> QueryExecutor::insertNewSession(const QSharedPointer<Session>& session)
{
    QSqlQuery query(m_database);
    qlonglong newId = nextSessionKey();
    QString newSessionToken = generateNewToken(session->getLastAccessTime().toUTC().toString(),
                                               session->getUser()->getEmail(),
                                               session->getUser()->getLogin(),
                                               session->getUser()->getPassword());

    syslog(LOG_INFO, "NewId ready, now preparing sql query for adding new session");
    query.prepare("insert into sessions (id, user_id, session_token, last_access_time) values (:id, :user_id, :token, :time);");
    query.bindValue(":id", newId);
    query.bindValue(":user_id", session->getUser()->getId());
    query.bindValue(":token", newSessionToken);
    query.bindValue(":time", session->getLastAccessTime().toUTC());

    m_database.transaction();

    bool result = query.exec();
    if (!result) {
        syslog(LOG_INFO,"Rollback for NewSession sql query");
        m_database.rollback();
        return QSharedPointer<Session>(NULL);
    } else {
        syslog(LOG_INFO,"Commit for NewSession sql query - insert in table sessions");
        m_database.commit();
    }
    return QSharedPointer<Session>(new DbSession(newId, newSessionToken, session->getLastAccessTime(), session->getUser()));
}

bool QueryExecutor::updateSession(const QSharedPointer<Session>& session)
{
    QSqlQuery query(m_database);
    QDateTime currentTime = QDateTime::currentDateTime().toUTC();
    syslog(LOG_INFO, "Updating session with token: %s", session->getSessionToken().toStdString().c_str());

    query.prepare("update sessions set last_access_time = :time where session_token = :token;");
    query.bindValue(":time", currentTime);
    query.bindValue(":token", session->getSessionToken());

    m_database.transaction();

    bool result = query.exec();
    if (!result) {
        syslog(LOG_INFO,"Rollback for updateSession sql query");
        m_database.rollback();
        return false;
    } else {
        syslog(LOG_INFO,"Commit for updateSession sql query");
        m_database.commit();
        session->setLastAccessTime(currentTime);
        return true;
    }
}

bool QueryExecutor::deleteSession(const QSharedPointer<Session> &session)
{
    QSqlQuery query(m_database);
    syslog(LOG_INFO, "Deleting session with token: %s", session->getSessionToken().toStdString().c_str());

    query.prepare("delete from sessions where id = :id;");
    query.bindValue(":id", session->getId());

    m_database.transaction();

    bool result = query.exec();
    if (!result) {
        syslog(LOG_INFO, "Rollback for deleteSession sql query");
        m_database.rollback();
        return false;
    } else {
        syslog(LOG_INFO, "Commit for deleteSession sql query");
        m_database.commit();
        return true;
    }
}
