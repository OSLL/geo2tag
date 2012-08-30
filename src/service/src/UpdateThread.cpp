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
#include "Geo2tagDatabase.h"

UpdateThread::UpdateThread(const QSharedPointer<DataMarks> &tags,
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
m_queryExecutor(0),
m_transactionCount(0)
{
}


UpdateThread::UpdateThread(const QSharedPointer<DataMarks> &tags,
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


bool UpdateThread::compareTransactionNumber(qlonglong factCount)
{
  bool result;
  syslog(LOG_INFO, "Checking number of write requests: logged = %lld, fact = %lld", m_transactionCount, factCount);
  // If m_transactionCount < transactionCount then need sync
  SettingsStorage storage(SETTINGS_STORAGE_FILENAME);
  qlonglong transactionDiff =  storage.getValue("General_Settings/transaction_diff", QVariant(DEFAULT_TRANSACTION_DIFF_TO_SYNC)).toLongLong();
  syslog(LOG_INFO, "Diff from config = %lld, fact = %lld", transactionDiff, factCount - m_transactionCount);
  result = (factCount - m_transactionCount >= transactionDiff);
  if (result) m_transactionCount = factCount;

  return result;
}


void UpdateThread::run()
{

  for(;;)
  {
    SettingsStorage storage(SETTINGS_STORAGE_FILENAME);
    qlonglong interval = storage.getValue("General_Settings/db_update_interval", QVariant(DEFAULT_DB_UPDATE_INTERVAL)).toLongLong();
    {
      PerformanceCounter counter("db_update");

      syslog(LOG_INFO, "trying to connect to database..., file: %s, line: %d", __FILE__, __LINE__);
      bool result = m_queryExecutor->connect();
      if(!result)
      {
        syslog(LOG_INFO, "connection error %s", m_queryExecutor->lastError().text().toStdString().c_str());
        QThread::msleep(1000);
        continue;
      }

      qDebug() << "connected...";
      // Check if DB contain new changes
      qlonglong oldTagsContainerSize = m_tagsContainer->size();
      qlonglong factCount = m_queryExecutor->getFactTransactionNumber();
      if (!compareTransactionNumber(factCount))
      {
        QThread::msleep(interval);
        continue;
      }

      m_queryExecutor->checkTmpUsers();
      m_queryExecutor->checkSessions(this);

      common::Users       usersContainer(*m_usersContainer);
      DataMarks   tagsContainer(*m_tagsContainer);
      Channels    channelsContainer(*m_channelsContainer);
      Sessions    sessionsContainer(*m_sessionsContainer);

      m_queryExecutor->loadUsers(usersContainer);
      m_queryExecutor->loadChannels(channelsContainer);
      m_queryExecutor->loadTags(tagsContainer);
      m_queryExecutor->loadSessions(sessionsContainer);

      lockWriting();
      syslog(LOG_INFO,"Containers locked for db_update");

      m_usersContainer->merge(usersContainer);
      m_tagsContainer->merge(tagsContainer);
      m_channelsContainer->merge(channelsContainer);
      m_sessionsContainer->merge(sessionsContainer);

      m_queryExecutor->updateReflections(*m_tagsContainer,*m_usersContainer, *m_channelsContainer, *m_sessionsContainer);

      syslog(LOG_INFO, "tags added. trying to unlock");
      unlockWriting();

      if (oldTagsContainerSize != m_tagsContainer->size())
      {
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
        Q_EMIT newTagInsertionComplete(m_tagsContainer->size()-oldTagsContainerSize);
      }
      syslog(LOG_INFO, "current users' size = %d",m_usersContainer->size());
      syslog(LOG_INFO, "current tags' size = %d",m_tagsContainer->size());
      syslog(LOG_INFO,  "current channels' size = %d", m_channelsContainer->size());
      syslog(LOG_INFO,  "current sessions' size = %d", m_sessionsContainer->size());

      m_queryExecutor->disconnect();
      qDebug() << "sync completed!!!";
      Q_EMIT syncronizationComplete();
    }
    QThread::msleep(interval);
  }
}
