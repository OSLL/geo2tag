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
#include <QDebug>
#include <QSettings>
#include "UpdateThread.h"
#include "SettingsStorage.h"
#include "defines.h"
#include "PerformanceCounter.h"
#include "QueryExecutor.h"

UpdateThread::UpdateThread(const QSqlDatabase &db,
                           const QSharedPointer<DataMarks> &tags,
                           const QSharedPointer<common::Users> &users,
                           const QSharedPointer<Channels> &channels,
                           const QSharedPointer<DataChannels>& dataChannelsMap,
                           const QSharedPointer<Sessions>& sessions,
                           QObject *parent)
    : QThread(parent),
      m_channelsContainer(channels),
      m_tagsContainer(tags),
      m_usersContainer(users),
      m_dataChannelsMap(dataChannelsMap),
      m_sessionsContainer(sessions),
      m_database(db),
      m_queryExecutor(0),
      m_transactionCount(0)
{
}

UpdateThread::UpdateThread(const QSqlDatabase &db,
                           const QSharedPointer<DataMarks> &tags,
                           const QSharedPointer<common::Users> &users,
                           const QSharedPointer<Channels> &channels,
                           const QSharedPointer<DataChannels>& dataChannelsMap,
                           const QSharedPointer<Sessions>& sessions,
                           QueryExecutor* queryExecutor,
                           QObject *parent)
    : QThread(parent),
      m_channelsContainer(channels),
      m_tagsContainer(tags),
      m_usersContainer(users),
      m_dataChannelsMap(dataChannelsMap),
      m_sessionsContainer(sessions),
      m_database(db),
      m_queryExecutor(queryExecutor),
      m_transactionCount(0)
{
}


void UpdateThread::lockWriting()
{
  m_updateLock.lockForWrite();
}


void UpdateThread::unlockWriting()
{
  m_updateLock.unlock();
}

void UpdateThread::setQueryExecutor(QueryExecutor *queryExecutor)
{
  m_queryExecutor = queryExecutor;
}

QSharedPointer<Sessions> UpdateThread::getSessionsContainer() const
{
  return m_sessionsContainer;
}

void UpdateThread::incrementTransactionCount(int i)
{
  m_transactionCount+=i;
  syslog(LOG_INFO, "Number of write requests: logged = %lld",m_transactionCount);
}


bool UpdateThread::compareTransactionNumber()
{
// Calculate number of successful write requests
  QSqlQuery query(m_database);
  bool result;
  query.exec("select tup_inserted ,tup_updated ,tup_deleted from  pg_stat_database where datname='geo2tag';");
  query.next();
  qlonglong transactionCount = query.record().value("tup_inserted").toLongLong() +
				query.record().value("tup_updated").toLongLong() +
				query.record().value("tup_deleted").toLongLong();

  syslog(LOG_INFO, "Checking number of write requests: logged = %lld, fact = %lld", m_transactionCount, transactionCount);
// If m_transactionCount < transactionCount then need sync 
  SettingsStorage storage(SETTINGS_STORAGE_FILENAME);
  qlonglong transactionDiff =  storage.getValue("General_Settings/transaction_diff", QVariant(DEFAULT_DB_UPDATE_INTERVAL)).toLongLong();
  syslog(LOG_INFO, "Diff from config = %lld, fact = %lld",transactionDiff,transactionCount-m_transactionCount);
  result = ( transactionCount - m_transactionCount >= transactionDiff);
  if (result) m_transactionCount = transactionCount;

  return result;
}

void UpdateThread::run()
{

  for(;;)
  {
    SettingsStorage storage(SETTINGS_STORAGE_FILENAME);
    qlonglong interval = storage.getValue("General_Settings/db_update_interval", QVariant(DEFAULT_TRANSACTION_DIFF_TO_SYNC)).toLongLong();
    {
    PerformanceCounter counter("db_update");    
    syslog(LOG_INFO, "trying to connect to database..., file: %s, line: %d", __FILE__, __LINE__);
    bool result = m_database.open();
    if(!result)
    {
      syslog(LOG_INFO, "connection error %s",m_database.lastError().text().toStdString().c_str());
      QThread::msleep(1000);
      continue;
    }

    qDebug() << "connected...";
// Check if DB contain new changes
    m_queryExecutor->checkTmpUsers();
    m_queryExecutor->checkSessions();
    if (!compareTransactionNumber())
    {
      QThread::msleep(interval);
      continue;
    }


    common::Users       usersContainer(*m_usersContainer);
    DataMarks   tagsContainer(*m_tagsContainer);
    Channels    channelsContainer(*m_channelsContainer);
    Sessions    sessionsContainer(*m_sessionsContainer);

    loadUsers(usersContainer);
    loadChannels(channelsContainer);
    loadTags(tagsContainer);
    loadSessions(sessionsContainer);

    lockWriting();
    syslog(LOG_INFO,"Containers locked for db_update");
    m_usersContainer->merge(usersContainer);
    m_tagsContainer->merge(tagsContainer);
    m_channelsContainer->merge(channelsContainer);
    m_sessionsContainer->merge(sessionsContainer);

    updateReflections(*m_tagsContainer,*m_usersContainer, *m_channelsContainer, *m_sessionsContainer);
    
    syslog(LOG_INFO, "tags added. trying to unlock");
    unlockWriting();

    syslog(LOG_INFO,"lock: filling m_dataChannelsMap ");
    for(int i=0; i<m_tagsContainer->size(); i++)
    {
      if(!m_dataChannelsMap->contains(m_tagsContainer->at(i)->getChannel(), m_tagsContainer->at(i)))
      {
        QSharedPointer<DataMark> tag = m_tagsContainer->at(i);
        QSharedPointer<Channel> channel = tag->getChannel();
        syslog(LOG_INFO, "adding %d from %d tag %s to channel %s", i, m_tagsContainer->size(),
         tag->getTime().toString("dd MM yyyy HH:mm:ss.zzz").toStdString().c_str(), channel->getName().toStdString().c_str());
        lockWriting();
        m_dataChannelsMap->insert(channel, tag);
        unlockWriting();
      }
    }


    syslog(LOG_INFO, "current users' size = %d",m_usersContainer->size());
    syslog(LOG_INFO, "current tags' size = %d",m_tagsContainer->size());
    syslog(LOG_INFO,  "current channels' size = %d", m_channelsContainer->size());
    syslog(LOG_INFO,  "current sessions' size = %d", m_sessionsContainer->size());
    m_database.close();
    }
    QThread::msleep(interval);
  }
}


void UpdateThread::loadUsers(common::Users &container)
{
  QSqlQuery query(m_database);
  query.exec("select id, login, password from users order by id;");
  while (query.next())
  {
    qlonglong id = query.record().value("id").toLongLong();
    if(container.exist(id))
    {
      // skip record
      continue;
    }
    QString login = query.record().value("login").toString();
    QString password = query.record().value("password").toString();
    syslog(LOG_INFO,"Pushing | %lld | %s ",id,login.toStdString().c_str());
    DbUser *newUser = new DbUser(login,password,id);
    QSharedPointer<DbUser> pointer(newUser);
    container.push_back(pointer);
  }
}


void UpdateThread::loadChannels(Channels &container)
{
  QSqlQuery query(m_database);
  query.exec("select id, description, name, url from channel order by id;");
  while (query.next())
  {
    qlonglong id = query.record().value("id").toLongLong();
    if(container.exist(id))
    {
      // skip record
      continue;
    }
    QString name = query.record().value("name").toString();
    QString description = query.record().value("description").toString();
    QString url = query.record().value("url").toString();
    QSharedPointer<DbChannel> pointer(new DbChannel(id,name,description,url));
    container.push_back(pointer);
  }
}



void UpdateThread::loadTags(DataMarks &container)
{
  QSqlQuery query(m_database);
  query.exec("select id, time, altitude, latitude, longitude, label, description, url, user_id, channel_id from tag order by time;");
  while (query.next())
  {
    qlonglong id = query.record().value("id").toLongLong();
    if(container.exist(id))
    {
      // skip record
      continue;
    }
    QDateTime time = query.record().value("time").toDateTime().toTimeSpec(Qt::LocalTime);
    //       // syslog(LOG_INFO, "loaded tag with time: %s milliseconds", time;
    qreal latitude = query.record().value("latitude").toReal();
    qreal altitude = query.record().value("altitude").toReal();
    qreal longitude = query.record().value("longitude").toReal();
    QString label = query.record().value("label").toString();
    QString description = query.record().value("description").toString();
    QString url = query.record().value("url").toString();
    qlonglong userId = query.record().value("user_id").toLongLong();
    qlonglong channelId = query.record().value("channel_id").toLongLong();

    DbDataMark *newMark = new DbDataMark(id,
      altitude,
      latitude,
      longitude,
      label,
      description,
      url,
      time,
      userId,
      channelId);
    QSharedPointer<DbDataMark> pointer(newMark);
    container.push_back(pointer);
  }
}

void UpdateThread::loadSessions(Sessions &container)
{
    syslog(LOG_INFO, "Sessions size = %d", m_usersContainer->size());
    QSqlQuery query(m_database);
    query.exec("select id, user_id, session_token, last_access_time from sessions;");
    while (query.next()) {
        qlonglong id = query.record().value("id").toLongLong();
        if (container.exist(id))
            continue;
        qlonglong userId = query.record().value("user_id").toLongLong();
        QSharedPointer<common::User> user(new DbUser( userId));

        QString sessionToken = query.record().value("session_token").toString();
        QDateTime lastAccessTime = query.record().value("last_access_time").toDateTime();//.toTimeSpec(Qt::LocalTime);

        QSharedPointer<Session> newSession(new DbSession(id, sessionToken, lastAccessTime, user));
        container.push_back(newSession);
    }
}

void UpdateThread::updateReflections(DataMarks &tags, common::Users &users, Channels &channels, Sessions &sessions)
{
  {
    QSqlQuery query(m_database);
    query.exec("select user_id, channel_id from subscribe;");
    while (query.next())
    {
      qlonglong user_id = query.record().value("user_id").toLongLong();
      qlonglong channel_id = query.record().value("channel_id").toLongLong();
      users.item(user_id)->subscribe(channels.item(channel_id));
    }
  }
  {
    QSqlQuery query(m_database);
    query.exec("select tag_id, channel_id from tags;");
    while (query.next())
    {
      qlonglong tag_id = query.record().value("tag_id").toLongLong();
      qlonglong channel_id = query.record().value("channel_id").toLongLong();

      QSharedPointer<Channel> channel = channels.item(channel_id);
      QSharedPointer<DataMark> tag = tags.item(tag_id);

      tag->setChannel(channel);
    }
  }

  for(int i=0; i<tags.size(); i++)
  {
    tags[i]->setUser(users.item(tags.at(i).dynamicCast<DbDataMark>()->getUserId()));
    tags[i]->setChannel(channels.item(tags.at(i).dynamicCast<DbDataMark>()->getChannelId()));
  }


  for(int i=0; i<sessions.size(); i++)
  {
      sessions[i]->setUser(users.item(sessions[i]->getUser()->getId()));
  }
}
