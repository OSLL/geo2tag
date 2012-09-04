/*
 * Copyright 2010  OSLL osll@osll.spb.ru
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
/*! ---------------------------------------------------------------
 *
 *
 * \file DbSession.cpp
 * \brief DbSession implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include <syslog.h>
#include <stdlib.h>
#include "defines.h"
#include "SettingsStorage.h"
#include "DbSession.h"
#include "EmailMessage.h"

#include "LoginRequestJSON.h"
#include "LoginResponseJSON.h"

#include "RegisterUserRequestJSON.h"
#include "RegisterUserResponseJSON.h"

#include "QuitSessionRequestJSON.h"
#include "QuitSessionResponseJSON.h"

#include "WriteTagRequestJSON.h"
#include "WriteTagResponseJSON.h"

#include "AddUserRequestJSON.h"
#include "AddUserResponseJSON.h"

#include "LoadTagsRequestJSON.h"
#include "LoadTagsResponseJSON.h"

#include "AddChannelRequestJSON.h"
#include "AddChannelResponseJSON.h"

#include "OwnedChannelsRequest.h"
#include "OwnedChannelsResponse.h"

#include "SubscribedChannelsRequestJSON.h"
#include "SubscribedChannelsResponseJSON.h"

#include "SubscribeChannelJSON.h"
#include "SubscribeChannelResponseJSON.h"

#include "AvailableChannelsRequestJSON.h"
#include "AvailableChannelsResponseJSON.h"

#include "UnsubscribeChannelRequestJSON.h"
#include "UnsubscribeChannelResponseJSON.h"

#include "RestorePasswordRequestJSON.h"
#include "RestorePasswordResponseJSON.h"

#include "Filtration.h"
#include "Filter.h"
#include "TimeFilter.h"
#include "ShapeFilter.h"
#include "AltitudeFilter.h"

#include "FilterDefaultResponseJSON.h"
#include "FilterCircleRequestJSON.h"
#include "FilterCylinderRequestJSON.h"
#include "FilterPolygonRequestJSON.h"
#include "FilterRectangleRequestJSON.h"
#include "FilterBoxRequestJSON.h"
#include "FilterFenceRequestJSON.h"

#include "FilterChannelRequestJSON.h"
#include "FilterChannelResponseJSON.h"

#include "DeleteUserRequestJSON.h"
#include "DeleteUserResponseJSON.h"

#include "ErrnoInfoResponseJSON.h"

#include "VersionResponseJSON.h"
#include "BuildResponseJSON.h"

#include "ChannelInternal.h"
#include "ErrnoTypes.h"

#include <QtSql>
#include <QMap>
#include <QRegExp>

#include "PerformanceCounter.h"
#include "Geo2tagDatabase.h"

namespace common
{
  const QString DbObjectsCollection::error = QString("Error");
  const QString DbObjectsCollection::ok = QString("Ok");

  DbObjectsCollection::DbObjectsCollection():
  m_channelsContainer(new Channels()),
    m_tagsContainer(new DataMarks()),
    m_usersContainer(new Users()),
    m_dataChannelsMap(new DataChannels()),
    m_sessionsContainer(new Sessions()),
    m_updateThread(NULL),
    m_queryExecutor(NULL)
  {

    m_processors.insert("login", &DbObjectsCollection::processLoginQuery);
    m_processors.insert("registerUser", &DbObjectsCollection::processRegisterUserQuery);
    m_processors.insert("quitSession", &DbObjectsCollection::processQuitSessionQuery);
    m_processors.insert("writeTag", &DbObjectsCollection::processWriteTagQuery);
    m_processors.insert("loadTags", &DbObjectsCollection::processLoadTagsQuery);
    m_processors.insert("subscribe", &DbObjectsCollection::processSubscribeQuery);
    m_processors.insert("owned", &DbObjectsCollection::processOwnedChannelsQuery);
    m_processors.insert("subscribed", &DbObjectsCollection::processSubscribedChannelsQuery);
    m_processors.insert("addUser", &DbObjectsCollection::processAddUserQuery);
    m_processors.insert("addChannel", &DbObjectsCollection::processAddChannelQuery);
    m_processors.insert("channels", &DbObjectsCollection::processAvailableChannelsQuery);
    m_processors.insert("unsubscribe", &DbObjectsCollection::processUnsubscribeQuery);
    m_processors.insert("version", &DbObjectsCollection::processVersionQuery);
    m_processors.insert("deleteUser", &DbObjectsCollection::processDeleteUserQuery);
    m_processors.insert("build", &DbObjectsCollection::processBuildQuery);
    m_processors.insert("restorePassword", &DbObjectsCollection::processRestorePasswordQuery);

    m_processors.insert("errnoInfo", &DbObjectsCollection::processGetErrnoInfo);
    m_processors.insert("filterCircle", &DbObjectsCollection::processFilterCircleQuery);
    m_processors.insert("filterCylinder", &DbObjectsCollection::processFilterCylinderQuery);
    m_processors.insert("filterPolygon", &DbObjectsCollection::processFilterPolygonQuery);
    m_processors.insert("filterRectangle", &DbObjectsCollection::processFilterRectangleQuery);
    m_processors.insert("filterBox", &DbObjectsCollection::processFilterBoxQuery);
    m_processors.insert("filterFence", &DbObjectsCollection::processFilterFenceQuery);
    m_processors.insert("filterChannel", &DbObjectsCollection::processFilterChannelQuery);
    //  Here also should be something like
    //  m_processors.insert("confirmRegistration-*", &DbObjectsCollection::processFilterFenceQuery);

    QSqlDatabase database = QSqlDatabase::addDatabase("QPSQL");
    database.setHostName("localhost");
    database.setDatabaseName("geo2tag");
    database.setUserName("geo2tag");
    database.setPassword("geo2tag");

    m_updateThread = new UpdateThread(
      m_tagsContainer,
      m_usersContainer,
      m_channelsContainer,
      m_dataChannelsMap,
      m_sessionsContainer,
      NULL,
      NULL);

    m_queryExecutor = new QueryExecutor(Geo2tagDatabase(QSqlDatabase::cloneDatabase(database,"executor"),
      QSharedPointer<UpdateThread>(m_updateThread))
      );
    m_updateThread->setQueryExecutor(m_queryExecutor);

    m_updateThread->start();
  }

  DbObjectsCollection& DbObjectsCollection::getInstance()
  {
    static DbObjectsCollection s;
    return s;
  }

  DbObjectsCollection::~DbObjectsCollection()
  {
    m_updateThread->wait();
    delete m_updateThread;
    delete m_queryExecutor;
  }

  QByteArray DbObjectsCollection::process(const QString& queryType, const QByteArray& body)
  {
    if(!m_queryExecutor->isConnected())
    {
      m_queryExecutor->connect();
    }

    QList<QString> queries = m_processors.uniqueKeys();
    for (int i=0;i<queries.size();i++)
    {
      if (queryType.compare(queries[i],Qt::CaseInsensitive) == 0)
      {

        ProcessMethod method = m_processors.value(queries[i]);
        syslog(LOG_INFO,"calling %s processor %s",queryType.toStdString().c_str(),QString(body).toStdString().c_str());
        QByteArray aa;
        PerformanceCounter a(queryType.toStdString());
        aa = (*this.*method)(body);
        return aa;
      }
    }

    // Extra code for extracting token from url !
    QRegExp rx("confirmRegistration-([a-zA-Z0-9]+)");
    if (rx.exactMatch(queryType))
    {
      syslog(LOG_INFO, "Pattern matched!");
      const QString token = rx.cap(1);
      ProcessMethodWithStr method = &DbObjectsCollection::processConfirmRegistrationQuery;
      QByteArray aa;
      PerformanceCounter a("confirmRegistration");
      aa = (*this.*method)(token);
      return aa;

    }
    else
    {
      syslog(LOG_INFO, "Pattern not matched!");
    }
    // end of extra code !

    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    DefaultResponseJSON response;
    response.setErrno(INCORRECT_QUERY_NAME_ERROR);

    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;

  }

  // Check password quality params - length, usage of different symbol groups ([-=+_*&^%$#@a-z],[A-Z],[0-9])
  bool DbObjectsCollection::checkPasswordQuality(const QString& password) const
  {
    // If check is not enabled in config - all passwords are good
    SettingsStorage storage(SETTINGS_STORAGE_FILENAME);
    bool checkEnabled = storage.getValue("Security_Settings/password_quality_check", QVariant(DEFAULT_PASSWORD_QUALITY_CHECK)).toBool();
    if (!checkEnabled) return true;

    if ( password.size() < MINIMAL_PASSWORD_LENGTH )
      return false;

    // Check that all symbol groups below are used in password
    QRegExp smallLetters("[-=+_*&^%$#@a-z]+");
    QRegExp bigLetters("[A-Z]+");
    QRegExp numbers("[0-9]+");

    if (smallLetters.indexIn(password)==-1 || bigLetters.indexIn(password)==-1 || numbers.indexIn(password)==-1 )
      return false;

    return true;

  }

  const QString DbObjectsCollection::getPasswordHash(const QSharedPointer<User>& user) const
  {
    return getPasswordHash(user->getLogin(),user->getPassword());
  }

  const QString DbObjectsCollection::getPasswordHash(const QString & login,const QString & password) const
  {
    QString startStr = login + password + PASSWORD_SALT;
    QByteArray toHash = QCryptographicHash::hash(startStr.toUtf8(),QCryptographicHash::Sha1);
    QString result(toHash.toHex());
    syslog(LOG_INFO,"Calculating hash for password = %s",result.toStdString().c_str());
    return result;
  }

  const QString DbObjectsCollection::generateNewPassword(const QSharedPointer<common::User>& user) const
  {
    QDateTime time = QDateTime::currentDateTime().toUTC();
    QString log = time.toString() + user->getEmail() + user->getPassword();
    QByteArray toHash(log.toUtf8());
    toHash=QCryptographicHash::hash(log.toUtf8(),QCryptographicHash::Md5);
    QString result(toHash.toHex());
    syslog(LOG_INFO,"Password = %s",result.toStdString().c_str());
    return result;
  }

  QSharedPointer<User> DbObjectsCollection::findUser(const QSharedPointer<User> &dummyUser) const
  {
    QSharedPointer<User> realUser;      // Null pointer
    QVector<QSharedPointer<User> > currentUsers = m_usersContainer->vector();
    syslog(LOG_INFO, "checking user key: %s from %d known users", dummyUser->getLogin().toStdString().c_str(),
      currentUsers.size());
    if (!dummyUser->getLogin().isEmpty() && !dummyUser->getPassword().isEmpty())
    {
      for(int i=0; i<currentUsers.size(); i++)
      {
        if(QString::compare(currentUsers.at(i)->getLogin(), dummyUser->getLogin(), Qt::CaseInsensitive) == 0
          &&
          currentUsers.at(i)->getPassword() == getPasswordHash(currentUsers.at(i)->getLogin(),dummyUser->getPassword()))
          return currentUsers.at(i);
      }
    }
    return realUser;
  }

  QSharedPointer<User> DbObjectsCollection::findUser(const QString& email) const
  {
    QSharedPointer<User> realUser;      // Null pointer
    QVector<QSharedPointer<User> > currentUsers = m_usersContainer->vector();
    syslog(LOG_INFO, "checking user key: %s from %d known users", email.toStdString().c_str(),
      currentUsers.size());
    if (!email.isEmpty())
    {
      for(int i=0; i<currentUsers.size(); i++)
      {
        if(QString::compare(currentUsers.at(i)->getEmail(), email, Qt::CaseInsensitive) == 0)
          return currentUsers.at(i);
      }
    }
    return realUser;
  }

  QSharedPointer<Session> DbObjectsCollection::findSession(const QSharedPointer<Session>& dummySession) const
  {
    QVector< QSharedPointer<Session> > currentSessions = m_sessionsContainer->vector();
    syslog(LOG_INFO, "checking session key: %s from %d known session", dummySession->getSessionToken().toStdString().c_str(),
      currentSessions.size());
    if (!dummySession->getSessionToken().isEmpty())
    {
      for (int i=0; i<currentSessions.size(); i++)
      {
        if(currentSessions.at(i)->getSessionToken() == dummySession->getSessionToken())
          return currentSessions.at(i);
      }
    }
    return QSharedPointer<Session>(NULL);
  }

  QSharedPointer<Session> DbObjectsCollection::findSessionForUser(const QSharedPointer<User>& user) const
  {
    QVector< QSharedPointer<Session> > currentSessions = m_sessionsContainer->vector();
    syslog(LOG_INFO, "checking of session existence for user with name: %s", user->getLogin().toStdString().c_str());
    if (!user->getLogin().isEmpty() && !user->getPassword().isEmpty())
    {
      for (int i = 0; i < currentSessions.size(); i++)
      {
        if (QString::compare(currentSessions.at(i)->getUser()->getLogin(), user->getLogin(), Qt::CaseInsensitive) == 0
          &&
          currentSessions.at(i)->getUser()->getPassword() == user->getPassword())
          return currentSessions.at(i);
      }
    }
    return QSharedPointer<Session>(NULL);
  }

  QByteArray DbObjectsCollection::processRegisterUserQuery(const QByteArray &data)
  {
    RegisterUserRequestJSON request;
    RegisterUserResponseJSON response;
    QByteArray answer;
    answer.append("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }
    QSharedPointer<User> newTmpUser = request.getUsers()->at(0);
    QVector<QSharedPointer<User> > currentUsers = m_usersContainer->vector();
    for(int i=0; i<currentUsers.size(); i++)
    {
      if(QString::compare(currentUsers.at(i)->getLogin(), newTmpUser->getLogin(), Qt::CaseInsensitive) == 0)
      {
        response.setErrno(USER_ALREADY_EXIST_ERROR);
        answer.append(response.getJson());
        syslog(LOG_INFO, "answer: %s", answer.data());
        return answer;
      }
    }

    if(m_queryExecutor->doesUserWithGivenEmailExist(newTmpUser))
    {
      response.setErrno(EMAIL_ALREADY_EXIST_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    if(m_queryExecutor->doesTmpUserExist(newTmpUser))
    {
      response.setErrno(TMP_USER_ALREADY_EXIST_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    if ( !checkPasswordQuality(newTmpUser->getPassword()) )
    {
      response.setErrno(WEAK_PASSWORD_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    // Only password hashes are stored, so we change password of this user by password hash
    newTmpUser->setPassword(getPasswordHash(newTmpUser));
    QString confirmToken = m_queryExecutor->insertNewTmpUser(newTmpUser);
    if(confirmToken.isEmpty())
    {
      response.setErrno(INTERNAL_DB_ERROR);
      answer.append(response.getJson());
      syslog(LOG_INFO, "answer: %s", answer.data());
      return answer;
    }

    response.setErrno(SUCCESS);
    SettingsStorage storage(SETTINGS_STORAGE_FILENAME);
    QString serverUrl = storage.getValue("General_Settings/server_url", QVariant(DEFAULT_SERVER)).toString();
    response.setConfirmUrl(serverUrl+QString("service/confirmRegistration-")+confirmToken);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processConfirmRegistrationQuery(const QString &registrationToken)
  {
    QByteArray answer;
    answer.append("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    //syslog(LOG_INFO, "Confirming!");
    bool tokenExists = m_queryExecutor->doesRegistrationTokenExist(registrationToken);
    if (!tokenExists)
    {
      answer.append("Given token doesn't exist!");
      return answer;
    }

    QSharedPointer<User> newUser = m_queryExecutor->insertTmpUserIntoUsers(registrationToken);

    if (!newUser.isNull())
    {
      m_queryExecutor->deleteTmpUser(registrationToken);
      m_updateThread->lockWriting();
      m_usersContainer->push_back(newUser);
      m_updateThread->unlockWriting();
      answer.append("Congratulations!");
    }
    else
    {
      answer.append("Attempt of inserting user has failed!");
    }
    return answer;
  }

  QByteArray DbObjectsCollection::processLoginQuery(const QByteArray &data)
  {
    LoginRequestJSON request;
    LoginResponseJSON response;
    QByteArray answer;
    answer.append("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<User> dummyUser = request.getUsers()->at(0);
    QSharedPointer<User> realUser = findUser(dummyUser);

    if(realUser.isNull())
    {
      response.setErrno(INCORRECT_CREDENTIALS_ERROR);
    }
    else
    {
      QSharedPointer<Session> session = findSessionForUser(realUser);
      if (session.isNull())
      {
        QSharedPointer<Session> dummySession(new Session("", QDateTime::currentDateTime(), realUser));
        syslog(LOG_INFO, "Session hasn't been found. Generating of new Session.");
        QSharedPointer<Session> addedSession = m_queryExecutor->insertNewSession(dummySession);
        if (!addedSession)
        {
          response.setErrno(INTERNAL_DB_ERROR);
          answer.append(response.getJson());
          syslog(LOG_INFO, "answer: %s", answer.data());
          return answer;
        }
        m_updateThread->lockWriting();
        m_sessionsContainer->push_back(addedSession);
        m_updateThread->unlockWriting();
        response.addSession(addedSession);
      }
      else
      {
        syslog(LOG_INFO, "Session has been found. Session's token: %s", session->getSessionToken().toStdString().c_str());
        syslog(LOG_INFO, "Time before updating: %s", session->getLastAccessTime().toUTC().toString().toStdString().c_str());
        m_queryExecutor->updateSession(session);
        syslog(LOG_INFO, "Time after updating: %s", session->getLastAccessTime().toUTC().toString().toStdString().c_str());
        response.addSession(session);
      }
      response.setErrno(SUCCESS);
      //response.addUser(realUser);
    }

    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processQuitSessionQuery(const QByteArray &data)
  {
    QuitSessionRequestJSON request;
    QuitSessionResponseJSON response;
    QByteArray answer;
    answer.append("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QString sessionToken = request.getSessionToken();
    syslog(LOG_INFO, "Searching of session with token: %s", sessionToken.toStdString().c_str());
    QSharedPointer<Session> dummySession(new Session(sessionToken, QDateTime::currentDateTime(), QSharedPointer<User>(NULL)));
    QSharedPointer<Session> realSession = findSession(dummySession);

    if(realSession.isNull())
    {
      syslog(LOG_INFO, "Session hasn't been found.");
      response.setErrno(WRONG_TOKEN_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    syslog(LOG_INFO, "Session has been found. Deleting...");
    syslog(LOG_INFO, "Number of sessions before deleting: %d", m_sessionsContainer->size());
    bool result = m_queryExecutor->deleteSession(realSession);
    if (!result)
    {
      response.setErrno(INTERNAL_DB_ERROR);
      answer.append(response.getJson());
      syslog(LOG_INFO, "answer: %s", answer.data());
      return answer;
    }
    m_sessionsContainer->erase(realSession);
    syslog(LOG_INFO, "Number of sessions after deleting: %d", m_sessionsContainer->size());

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processWriteTagQuery(const QByteArray &data)
  {
    WriteTagRequestJSON request;
    WriteTagResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<DataMark> dummyTag = request.getTags()->at(0);
    syslog(LOG_INFO,"Adding mark with altitude = %f",dummyTag->getAltitude());

    QSharedPointer<Session> dummySession = request.getSessions()->at(0);
    syslog(LOG_INFO,"Checking for sessions with token = %s",dummySession->getSessionToken().toStdString().c_str());
    QSharedPointer<Session> realSession = findSession(dummySession);
    if(realSession.isNull())
    {
      response.setErrno(WRONG_TOKEN_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<User> realUser = realSession->getUser();

    QSharedPointer<Channel> dummyChannel = request.getChannels()->at(0);
    QSharedPointer<Channel> realChannel;// Null pointer
    QVector<QSharedPointer<Channel> > currentChannels = realUser->getSubscribedChannels()->vector();

    for(int i=0; i<currentChannels.size(); i++)
    {
      if(QString::compare(currentChannels.at(i)->getName(), dummyChannel->getName(), Qt::CaseInsensitive) == 0)
      {
        realChannel = currentChannels.at(i);
      }
    }

    //bool channelSubscribed = m_queryExecutor->isChannelSubscribed(dummyChannel, realUser);
    if(realChannel.isNull())
    {
      response.setErrno(CHANNEL_NOT_SUBCRIBED_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    dummyTag->setChannel(realChannel);
    dummyTag->setSession(realSession);
    dummyTag->setUser(realUser);
                                        //now
    QSharedPointer<DataMark> realTag = m_queryExecutor->insertNewTag(dummyTag);
    if(realTag == NULL)
    {
      response.setErrno(INTERNAL_DB_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    m_updateThread->lockWriting();
    m_tagsContainer->push_back(realTag);
    m_dataChannelsMap->insert(realChannel, realTag);
    m_updateThread->unlockWriting();

    syslog(LOG_INFO, "Time before updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());
    m_queryExecutor->updateSession(realSession);
    syslog(LOG_INFO, "Time after updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());

    response.setErrno(SUCCESS);
    response.addTag(realTag);

    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processOwnedChannelsQuery(const QByteArray &data)
  {
    OwnedChannelsRequestJSON request;
    OwnedChannelsResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }
    QSharedPointer<Session> dummySession = request.getSessions()->at(0);
    QSharedPointer<Session> realSession = findSession(dummySession);
    if(realSession.isNull())
    {
      response.setErrno(WRONG_TOKEN_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<User> realUser = realSession->getUser();

    QSharedPointer<Channels> ownedChannels(new Channels());
    QVector< QSharedPointer<Channel> > currentChannels = m_channelsContainer->vector();
    for(int i=0; i<currentChannels.size(); i++)
    {
      if(realUser->getId() == currentChannels.at(i)->getOwner()->getId())
      {
        ownedChannels->push_back(currentChannels.at(i));
      }
    }

    syslog(LOG_INFO, "Time before updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());
    m_queryExecutor->updateSession(realSession);
    syslog(LOG_INFO, "Time after updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());

    response.setChannels(ownedChannels);
    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processSubscribedChannelsQuery(const QByteArray &data)
  {
    SubscribedChannelsRequestJSON request;
    SubscribedChannelsResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }
    QSharedPointer<Session> dummySession = request.getSessions()->at(0);
    QSharedPointer<Session> realSession = findSession(dummySession);
    if(realSession.isNull())
    {
      response.setErrno(WRONG_TOKEN_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<User> realUser = realSession->getUser();

    QSharedPointer<Channels> channels = realUser->getSubscribedChannels();
    response.setChannels(channels);

    syslog(LOG_INFO, "Time before updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());
    m_queryExecutor->updateSession(realSession);
    syslog(LOG_INFO, "Time after updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processLoadTagsQuery(const QByteArray &data)
  {
    LoadTagsRequestJSON request;
    LoadTagsResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }
    QSharedPointer<Session> dummySession = request.getSessions()->at(0);
    QSharedPointer<Session> realSession = findSession(dummySession);
    if(realSession.isNull())
    {
      response.setErrno(WRONG_TOKEN_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<User> realUser = realSession->getUser();
    QSharedPointer<Channels> channels = realUser->getSubscribedChannels();
    DataChannels feed;

    double lat1 = request.getLatitude();
    double lon1 = request.getLongitude();
    double radius  = request.getRadius();
    //        syslog(LOG_INFO, "rssfeed processing: user %s has %d channels subscribed",
    //               realUser->getLogin().toStdString().c_str(), channels->size());
    for(int i = 0; i<channels->size(); i++)
    {
      QSharedPointer<Channel> channel = channels->at(i);
      QList<QSharedPointer<DataMark> > tags = m_dataChannelsMap->values(channel);
      qSort(tags);
      for(int j = 0; j < tags.size(); j++)
      {
        QSharedPointer<DataMark> mark = tags.at(j);
        double lat2 = mark->getLatitude();
        double lon2 = mark->getLongitude();

        if ( DataMark::getDistance(lat1, lon1, lat2, lon2) < radius )
          feed.insert(channel, mark);
      }
    }
    response.setData(feed);

    syslog(LOG_INFO, "Time before updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());
    m_queryExecutor->updateSession(realSession);
    syslog(LOG_INFO, "Time after updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  //TODO create function that will check validity of authkey, and channel name
  QByteArray DbObjectsCollection::processSubscribeQuery(const QByteArray &data)
  {
    syslog(LOG_INFO, "starting SubscribeQuery processing");
    SubscribeChannelRequestJSON request;
    SubscribeChannelResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<Session> dummySession = request.getSessions()->at(0);;
    QSharedPointer<Session> realSession = findSession(dummySession);
    if(realSession.isNull())
    {
      response.setErrno(WRONG_TOKEN_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<User> realUser = realSession->getUser();

    QSharedPointer<Channel> dummyChannel = request.getChannels()->at(0);;

    QSharedPointer<Channel> realChannel;// Null pointer
    QVector<QSharedPointer<Channel> > currentChannels = m_channelsContainer->vector();
    for(int i=0; i<currentChannels.size(); i++)
    {
      if(QString::compare(currentChannels.at(i)->getName(), dummyChannel->getName(), Qt::CaseInsensitive) == 0)
      {
        realChannel = currentChannels.at(i);
      }
    }
    if(realChannel.isNull())
    {
      response.setErrno(CHANNEL_DOES_NOT_EXIST_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<Channels>  subscribedChannels = realUser->getSubscribedChannels();
    for(int i=0; i<subscribedChannels->size(); i++)
    {
      //syslog(LOG_INFO,"%s is subscribed for  %s , %lld",realUser->getLogin().toStdString().c_str(),subscribedChannels->at(i)->getName().toStdString().c_str(),subscribedChannels->at(i)->getId());
      if(QString::compare(subscribedChannels->at(i)->getName(), realChannel->getName(),Qt::CaseInsensitive) == 0)
      {
        response.setErrno(CHANNEL_ALREADY_SUBSCRIBED_ERROR);
        answer.append(response.getJson());
        return answer;
      }
    }
    syslog(LOG_INFO, "Sending sql request for SubscribeQuery");
    bool result = m_queryExecutor->subscribeChannel(realUser,realChannel);
    if(!result)
    {
      response.setErrno(INTERNAL_DB_ERROR);
      answer.append(response.getJson());
      return answer;
    }
    m_updateThread->lockWriting();
    syslog(LOG_INFO, "Try to subscribe for realChannel = %lld",realChannel->getId());
    realUser->subscribe(realChannel);
    m_updateThread->unlockWriting();

    syslog(LOG_INFO, "Time before updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());
    m_queryExecutor->updateSession(realSession);
    syslog(LOG_INFO, "Time after updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processAddUserQuery(const QByteArray &data)
  {
    syslog(LOG_INFO, "starting AddUser processing");
    AddUserRequestJSON request;
    syslog(LOG_INFO, " AddUserRequestJSON created, now create AddUserResponseJSON ");
    AddUserResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }
    // Look for user with the same name
    QSharedPointer<User> dummyUser = request.getUsers()->at(0);
    QVector<QSharedPointer<User> > currentUsers = m_usersContainer->vector();

    for(int i=0; i<currentUsers.size(); i++)
    {
      if(QString::compare(currentUsers.at(i)->getLogin(), dummyUser->getLogin(), Qt::CaseInsensitive) == 0)
      {
        response.setErrno(USER_ALREADY_EXIST_ERROR);
        answer.append(response.getJson());
        syslog(LOG_INFO, "answer: %s", answer.data());
        return answer;
      }
    }

    if(m_queryExecutor->doesUserWithGivenEmailExist(dummyUser))
    {
      response.setErrno(EMAIL_ALREADY_EXIST_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    if ( !checkPasswordQuality(dummyUser->getPassword()) )
    {
      response.setErrno(WEAK_PASSWORD_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    syslog(LOG_INFO, "Sending sql request for AddUser");
    // Only password hashes are stored, so we change password of this user by password hash
    dummyUser->setPassword(getPasswordHash(dummyUser));
    QSharedPointer<User> addedUser = m_queryExecutor->insertNewUser(dummyUser);

    if(!addedUser)
    {
      response.setErrno(INTERNAL_DB_ERROR);
      answer.append(response.getJson());
      syslog(LOG_INFO, "answer: %s", answer.data());
      return answer;
    }
    m_updateThread->lockWriting();
    // Here will be adding user into user container
    m_usersContainer->push_back(addedUser);
    m_updateThread->unlockWriting();

    response.addUser(addedUser);
    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;

  }

  QByteArray DbObjectsCollection::processAddChannelQuery(const QByteArray &data)
  {
    syslog(LOG_INFO, "starting AddChannelQuery processing");
    AddChannelRequestJSON request;
    AddChannelResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<Session> dummySession = request.getSessions()->at(0);
    QSharedPointer<Session> realSession = findSession(dummySession);
    if(realSession.isNull())
    {
      response.setErrno(WRONG_TOKEN_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<Channel> dummyChannel = request.getChannels()->at(0);
    QSharedPointer<Channel> realChannel;// Null pointer
    QVector<QSharedPointer<Channel> > currentChannels = m_channelsContainer->vector();
    for(int i=0; i<currentChannels.size(); i++)
    {
      if(QString::compare(currentChannels.at(i)->getName(), dummyChannel->getName(), Qt::CaseInsensitive) == 0)
      {
        realChannel = currentChannels.at(i);
      }
    }

    if(!realChannel.isNull())
    {
      response.setErrno(CHANNEL_ALREADY_EXIST_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    syslog(LOG_INFO, "Sending sql request for AddChannel");
    dummyChannel->setOwner(realSession->getUser());
    QSharedPointer<Channel> addedChannel = m_queryExecutor->insertNewChannel(dummyChannel);

    if(!addedChannel)
    {
      response.setErrno(INTERNAL_DB_ERROR);
      answer.append(response.getJson());
      syslog(LOG_INFO, "answer: %s", answer.data());
      return answer;
    }

    m_updateThread->lockWriting();
    // Here will be adding user into user container
    m_channelsContainer->push_back(addedChannel);
    m_updateThread->unlockWriting();

    syslog(LOG_INFO, "Time before updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());
    m_queryExecutor->updateSession(realSession);
    syslog(LOG_INFO, "Time after updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processAvailableChannelsQuery(const QByteArray &data)
  {
    AvailableChannelsRequestJSON request;
    AvailableChannelsResponseJSON response;
    syslog(LOG_INFO, "processAvailableChannelsQuery - data = %s", data.data());
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }
    QSharedPointer<Session> dummySession = request.getSessions()->at(0);
    QSharedPointer<Session> realSession = findSession(dummySession);
    if(realSession.isNull())
    {
      response.setErrno(WRONG_TOKEN_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    syslog(LOG_INFO, "Time before updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());
    m_queryExecutor->updateSession(realSession);
    syslog(LOG_INFO, "Time after updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());

    response.setChannels(m_channelsContainer);
    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processUnsubscribeQuery(const QByteArray &data)
  {
    UnsubscribeChannelRequestJSON request;
    UnsubscribeChannelResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }
    QSharedPointer<Session> dummySession = request.getSessions()->at(0);
    QSharedPointer<Session> realSession = findSession(dummySession);
    if(realSession.isNull())
    {
      response.setErrno(WRONG_TOKEN_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<User> realUser = realSession->getUser();

    QSharedPointer<Channel> dummyChannel = request.getChannels()->at(0);;

    QSharedPointer<Channel> realChannel;// Null pointer
    QSharedPointer<Channels> subscribedChannels = realUser->getSubscribedChannels();
    for(int i=0; i<subscribedChannels->size(); i++)
    {
      if(QString::compare(subscribedChannels->at(i)->getName(), dummyChannel->getName(), Qt::CaseInsensitive) == 0)
      {
        realChannel = subscribedChannels->at(i);
        break;
      }
    }
    if(realChannel.isNull())
    {
      response.setErrno(CHANNEL_NOT_SUBCRIBED_ERROR);
      answer.append(response.getJson());
      return answer;
    }
    syslog(LOG_INFO, "Sending sql request for UnsubscribeQuery");
    bool result = m_queryExecutor->unsubscribeChannel(realUser,realChannel);

    if(!result)
    {
      response.setErrno(INTERNAL_DB_ERROR);
      answer.append(response.getJson());
      return answer;
    }
    m_updateThread->lockWriting();
    realUser->unsubscribe(realChannel);
    m_updateThread->unlockWriting();

    syslog(LOG_INFO, "Time before updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());
    m_queryExecutor->updateSession(realSession);
    syslog(LOG_INFO, "Time after updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processFilterCircleQuery(const QByteArray& data)
  {
    syslog(LOG_INFO, ">>> processFilterCircleQuery");

    FilterCircleRequestJSON request;
    return internalProcessFilterQuery(request, data, false);
  }

  QByteArray DbObjectsCollection::processFilterCylinderQuery(const QByteArray& data)
  {
    FilterCylinderRequestJSON request;
    return internalProcessFilterQuery(request, data, true);
  }

  QByteArray DbObjectsCollection::processFilterPolygonQuery(const QByteArray& data)
  {
    FilterPolygonRequestJSON request;
    return internalProcessFilterQuery(request, data, false);
  }

  QByteArray DbObjectsCollection::processFilterRectangleQuery(const QByteArray& data)
  {
    FilterRectangleRequestJSON request;
    return internalProcessFilterQuery(request, data, false);
  }

  QByteArray DbObjectsCollection::processFilterBoxQuery(const QByteArray& data)
  {
    FilterBoxRequestJSON request;
    return internalProcessFilterQuery(request, data, true);
  }

  QByteArray DbObjectsCollection::processFilterFenceQuery(const QByteArray& data)
  {
    FilterFenceRequestJSON request;
    return internalProcessFilterQuery(request, data, true);
  }

  QByteArray DbObjectsCollection::internalProcessFilterQuery(FilterRequestJSON& request,
    const QByteArray& data, bool is3d)
  {
    Filtration filtration;
    FilterDefaultResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<Session> dummySession = request.getSessions()->at(0);
    QSharedPointer<Session> realSession = findSession(dummySession);
    if(realSession.isNull())
    {
      response.setErrno(WRONG_TOKEN_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<User> realUser = realSession->getUser();

    filtration.addFilter(QSharedPointer<Filter>(new ShapeFilter(request.getShape())));
    filtration.addFilter(QSharedPointer<Filter>(new TimeFilter(request.getTimeFrom(), request.getTimeTo())));
    if(is3d)
    {
      filtration.addFilter(QSharedPointer<Filter>(new AltitudeFilter(request.getAltitude1(), request.getAltitude2())));
    }

    QSharedPointer<Channels> channels = realUser->getSubscribedChannels();
    DataChannels feed;
    if (request.getChannels()->size() > 0)
    {
      syslog(LOG_INFO, "point_2");

      QSharedPointer<Channel> targetChannel;
      // look for ...
      for(int i = 0; i<channels->size(); i++)
      {
        if (channels->at(i)->getName() == request.getChannels()->at(0)->getName())
        {
          targetChannel = channels->at(i);
          break;
        }
      }

      if (targetChannel.isNull())
      {
        response.setErrno(CHANNEL_DOES_NOT_EXIST_ERROR);
        answer.append(response.getJson());
        syslog(LOG_INFO, "answer: %s", answer.data());
        return answer;
      }
      QList<QSharedPointer<DataMark> > tags = m_dataChannelsMap->values(targetChannel);
      QList<QSharedPointer<DataMark> > filteredTags = filtration.filtrate(tags);
      for(int i = 0; i < filteredTags.size(); i++)
      {
        feed.insert(targetChannel, filteredTags.at(i));
      }
      response.setErrno(SUCCESS);
    }
    else
    {
      for(int i = 0; i<channels->size(); i++)
      {
        QSharedPointer<Channel> channel = channels->at(i);
        QList<QSharedPointer<DataMark> > tags = m_dataChannelsMap->values(channel);
        QList<QSharedPointer<DataMark> > filteredTags = filtration.filtrate(tags);
        for(int j = 0; j < filteredTags.size(); j++)
        {
          feed.insert(channel, filteredTags.at(j));
        }
      }
      response.setErrno(SUCCESS);
    }
    response.setDataChannels(feed);

    syslog(LOG_INFO, "Time before updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());
    m_queryExecutor->updateSession(realSession);
    syslog(LOG_INFO, "Time after updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());

    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processGetErrnoInfo(const QByteArray&)
  {
    ErrnoInfoResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processVersionQuery(const QByteArray&)
  {
    VersionResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    SettingsStorage storage(SETTINGS_STORAGE_FILENAME);
    QString version = storage.getValue("General_Settings/geo2tag_version").toString();

    response.setErrno(SUCCESS);
    response.setVersion(version);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processBuildQuery(const QByteArray&)
  {
    BuildResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    SettingsStorage storage(SETTINGS_STORAGE_FILENAME);
    QString version = storage.getValue("General_Settings/geo2tag_build").toString();

    response.setErrno(SUCCESS);
    response.setVersion(version);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processFilterChannelQuery(const QByteArray& data)
  {
    FilterChannelRequestJSON request;
    FilterChannelResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<Session> dummySession = request.getSessions()->at(0);
    QSharedPointer<Session> realSession = findSession(dummySession);
    if(realSession.isNull())
    {
      response.setErrno(WRONG_TOKEN_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QSharedPointer<User> realUser = realSession->getUser();
    QSharedPointer<Channels> channels = realUser->getSubscribedChannels();
    QSharedPointer<Channel> channel;
    for(int i = 0; i<channels->size(); i++)
    {
      if(QString::compare(channels->at(i)->getName(), request.getChannelName(), Qt::CaseInsensitive) == 0)
      {
        channel = channels->at(i);
        break;
      }
    }
    if (channel.isNull())
    {
      response.setErrno(CHANNEL_DOES_NOT_EXIST_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    QList<QSharedPointer<DataMark> > tags = m_dataChannelsMap->values(channel);
    int amount = request.getAmount();
    tags = tags.count() > amount ? tags.mid(0, amount) : tags;

    syslog(LOG_INFO, "Time before updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());
    m_queryExecutor->updateSession(realSession);
    syslog(LOG_INFO, "Time after updating: %s", realSession->getLastAccessTime().toUTC().toString().toStdString().c_str());

    response.setData(channel, tags);
    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processDeleteUserQuery(const QByteArray& data)
  {
    syslog(LOG_INFO, "starting DeleteUser processing");
    DeleteUserRequestJSON request;
    DeleteUserResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }
    // Look for user with the same name
    QSharedPointer<User> realUser = findUser(request.getUsers()->at(0));

    if(!realUser)
    {
      response.setErrno(INCORRECT_CREDENTIALS_ERROR);
      answer.append(response.getJson());
      syslog(LOG_INFO, "answer: %s", answer.data());
      return answer;
    }

    syslog(LOG_INFO, "Sending sql request for DeleteUser");
    bool isDeleted = m_queryExecutor->deleteUser(realUser);

    if(!isDeleted)
    {
      response.setErrno(INTERNAL_DB_ERROR);
      answer.append(response.getJson());
      syslog(LOG_INFO, "answer: %s", answer.data());
      return answer;
    }
    m_updateThread->lockWriting();
    // Here will be removing user from container
    m_usersContainer->erase(realUser);
    m_updateThread->unlockWriting();

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }

  QByteArray DbObjectsCollection::processRestorePasswordQuery(const QByteArray& data)
  {
    syslog(LOG_INFO, "starting RestorePassword processing");
    RestorePasswordRequestJSON request;
    RestorePasswordResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }
    // Look for user with the same name
    QSharedPointer<User> realUser = findUser(request.getUsers()->at(0)->getEmail());

    if(!realUser)
    {
      response.setErrno(INCORRECT_CREDENTIALS_ERROR);
      answer.append(response.getJson());
      syslog(LOG_INFO, "answer: %s", answer.data());
      return answer;
    }

    SettingsStorage storage(SETTINGS_STORAGE_FILENAME);
    int passwLength = storage.getValue("Security_Settings/password_length", QVariant(DEFAULT_PASSWORD_LENGTH)).toInt();
    QString password = generateNewPassword(realUser).left(passwLength);
    QString hash = getPasswordHash(realUser->getLogin(), password);
    syslog(LOG_INFO, "%s", realUser->getPassword().toStdString().c_str());
    QSharedPointer<common::User> updatedUser = m_queryExecutor->updateUserPassword(realUser, hash);

    if(updatedUser.isNull())
    {
      response.setErrno(INTERNAL_DB_ERROR);
      answer.append(response.getJson());
      syslog(LOG_INFO, "answer: %s", answer.data());
      return answer;
    }

    syslog(LOG_INFO, "Sending email for restoring password");
    EmailMessage message(updatedUser->getEmail());
    message.sendAsRestorePwdMessage(password);

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    syslog(LOG_INFO, "answer: %s", answer.data());
    return answer;
  }
}                                       // namespace common


/* ===[ End of file ]=== */
