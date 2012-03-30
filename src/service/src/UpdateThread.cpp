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
#include "defines.h"

UpdateThread::UpdateThread(const QSqlDatabase &db,
const QSharedPointer<DataMarks> &tags,
const QSharedPointer<common::Users> &users,
const QSharedPointer<Channels> &channels,
const QSharedPointer<TimeSlots> &timeSlots,
const QSharedPointer<DataChannels>& dataChannelsMap,
QObject *parent):
QThread(parent),
m_channelsContainer(channels),
m_tagsContainer(tags),
m_usersContainer(users),
m_timeSlotsContainer(timeSlots),
m_dataChannelsMap(dataChannelsMap),
m_database(db)
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


void UpdateThread::run()
{
  for(;;)
  {
    syslog(LOG_INFO, "trying to connect to database..., file: %s, line: %d", __FILE__, __LINE__);
    bool result = m_database.open();
    if(!result)
    {
      syslog(LOG_INFO, "connection error %s",m_database.lastError().text().toStdString().c_str());
      QThread::msleep(1000);
      continue;
    }
    qDebug() << "connected...";
    common::Users       usersContainer(*m_usersContainer);
    DataMarks   tagsContainer(*m_tagsContainer);
    Channels    channelsContainer(*m_channelsContainer);
    TimeSlots   timeSlotsContainer(*m_timeSlotsContainer);

    checkTmpUsers(); // New!
    loadUsers(usersContainer);
    loadTags(tagsContainer);
    loadChannels(channelsContainer);
    loadTimeSlots(timeSlotsContainer);

    lockWriting();
    m_usersContainer->merge(usersContainer);
    m_tagsContainer->merge(tagsContainer);
    m_channelsContainer->merge(channelsContainer);
    m_timeSlotsContainer->merge(timeSlotsContainer);

    updateReflections(*m_tagsContainer,*m_usersContainer, *m_channelsContainer, *m_timeSlotsContainer);

    for(int i=0; i<m_tagsContainer->size(); i++)
    {
      QSharedPointer<DataMark> tag = m_tagsContainer->at(i);
      QSharedPointer<Channel> channel = tag->getChannel();
      if(!m_dataChannelsMap->contains(channel, tag))
      {
        // syslog(LOG_INFO, "adding %d from %d tag %s to channel %s", i, m_tagsContainer->size(),
        // tag->getTime().toString("dd MM yyyy HH:mm:ss.zzz").toStdString().c_str(), channel->getName().toStdString().c_str());

        m_dataChannelsMap->insert(channel, tag);
      }
    }
    syslog(LOG_INFO, "tags added. trying to unlock");
    unlockWriting();

    syslog(LOG_INFO, "current users' size = %d",m_usersContainer->size());
    syslog(LOG_INFO, "current tags' size = %d",m_tagsContainer->size());
    syslog(LOG_INFO,  "current channels' size = %d", m_channelsContainer->size());
    m_database.close();
    QThread::msleep(10000);
  }
}


void UpdateThread::loadUsers(common::Users &container)
{
  QSqlQuery query(m_database);
  query.exec("select id, login, password, token from users order by id;");
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
    QString token = query.record().value("token").toString();
    syslog(LOG_INFO,"Pushing | %lld | %s | %s ",id,login.toStdString().c_str(),token.toStdString().c_str());
    DbUser *newUser = new DbUser(login,password,id,token);
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
    DbChannel *newChannel = new DbChannel(id,name,description,url);
    QSharedPointer<DbChannel> pointer(newChannel);
    container.push_back(pointer);
  }
}


void UpdateThread::loadTimeSlots(TimeSlots &container)
{
  QSqlQuery query(m_database);
  query.exec("select id, slot from timeSlot order by id;");
  while (query.next())
  {
    qlonglong id = query.record().value("id").toLongLong();
    if(container.exist(id))
    {
      // skip record
      continue;
    }

    qulonglong slot = query.record().value("slot").toULongLong();
    if (slot == 0)
      syslog(LOG_INFO, "can't convert to qulonglong");

    syslog(LOG_INFO, "slot: %llu milliseconds", slot);

    DbTimeSlot * newTimeSlot = new DbTimeSlot(id, slot);
    QSharedPointer<DbTimeSlot> pointer(newTimeSlot);
    container.push_back(pointer);
  }
}


void UpdateThread::loadTags(DataMarks &container)
{
  QSqlQuery query(m_database);
  query.exec("select id, time, altitude, latitude, longitude, label, description, url, user_id from tag order by time;");
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

    DbDataMark *newMark = new DbDataMark(id,
      altitude,
      latitude,
      longitude,
      label,
      description,
      url,
      time,
      userId);
    QSharedPointer<DbDataMark> pointer(newMark);
    container.push_back(pointer);
  }
}


void UpdateThread::updateReflections(DataMarks &tags, common::Users &users, Channels &channels, TimeSlots & timeSlots)
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
  }

  {
    QSqlQuery query(m_database);
    query.exec("select channel_id, timeslot_id from channeltimeslot;");
    while (query.next())
    {
      qlonglong timeslot_id = query.record().value("timeslot_id").toLongLong();
      qlonglong channel_id = query.record().value("channel_id").toLongLong();

      QSharedPointer<Channel> channel = channels.item(channel_id);
      QSharedPointer<TimeSlot> timeslot = timeSlots.item(timeslot_id);

      channel->setTimeSlot(timeslot);
      channel->setDefaultTimeSlot(false);
    }
  }

  {
    QSqlQuery query(m_database);
    query.exec("select tag_id, timeslot_id from tagtimeslot;");
    while (query.next())
    {
      qulonglong timeslot_id = query.record().value("timeslot_id").toULongLong();
      qlonglong tag_id = query.record().value("tag_id").toLongLong();

      QSharedPointer<DataMark> tag = tags.item(tag_id);
      QSharedPointer<TimeSlot> timeslot = timeSlots.item(timeslot_id);

      tag->setTimeSlot(timeslot);
    }
  }

  {
    QSqlQuery query(m_database);
    query.exec("select tag_id, timeslot_id from tagtimeslot;");
    while (query.next())
    {
      qulonglong timeslot_id = query.record().value("timeslot_id").toULongLong();
      qlonglong tag_id = query.record().value("tag_id").toLongLong();

      QSharedPointer<DataMark> tag = tags.item(tag_id);
      QSharedPointer<TimeSlot> timeslot = timeSlots.item(timeslot_id);

      tag->setTimeSlot(timeslot);
    }
  }

}

void UpdateThread::checkTmpUsers()
{
    QSqlQuery checkQuery(m_database);
    QSqlQuery deleteQuery(m_database);
    syslog(LOG_INFO,"checkTmpUsers query is running now...");
    // Sending emails to new users
    checkQuery.exec("select id, email, registration_token from signups where sent = false;");
    while (checkQuery.next()) {
        qlonglong id = checkQuery.value(0).toLongLong();
        QString email = checkQuery.value(1).toString();
        QString token = checkQuery.value(2).toString();
        sendConfirmationLetter(email, token);
        checkQuery.prepare("update signups set sent = true where id = :id;");
        checkQuery.bindValue(":id", id);
        checkQuery.exec();
    }

    // Deleting old signups
    QString strQuery;
    strQuery.append("select id from signups where (now() - datetime) >= INTERVAL '");
    strQuery.append(DEFAULT_TMP_USER_TIMELIFE);
    strQuery.append("';");
    checkQuery.exec(strQuery.toStdString().c_str());
    while (checkQuery.next()) {
        qlonglong id = checkQuery.value(0).toLongLong();
        deleteQuery.prepare("delete from signups where id = :id;");
        deleteQuery.bindValue(":id", id);
        syslog(LOG_INFO,"Deleting: %s", deleteQuery.lastQuery().toStdString().c_str());
        m_database.transaction();
        bool result = deleteQuery.exec();
        if(!result) {
            syslog(LOG_INFO,"Rollback for DeleteTmpUser sql query");
            m_database.rollback();
        } else {
            syslog(LOG_INFO,"Commit for DeleteTmpUser sql query");
            m_database.commit();
        }
    }
}

void UpdateThread::sendConfirmationLetter(const QString &address, const QString &token)
{
    QSettings settings(QSettings::SystemScope,"osll","libs");
    if (settings.value("mail_subject").toString().isEmpty()) {
        settings.setValue("mail_subject", "Registration confirmation");
    }
    syslog(LOG_INFO, "Process registration confirmation is started... ");
    QString subject = settings.value("mail_subject").toString();
    QString body = "'This will go into the body of the mail.'";
    body.append("To confirm registration, please, go to this link: ");
    body.append(DEFAULT_SERVER);
    body.append("service/confirmRegistration-");
    body.append(token);
    QString command = "echo " + body + " | mail -s '" + subject + "' " + address;
    system(command.toStdString().c_str());
    syslog(LOG_INFO, "Process registration confirmation finished... ");
}
