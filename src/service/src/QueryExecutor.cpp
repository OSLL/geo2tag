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

#include "DataMarkInternal.h"
#include "UserInternal.h"
#include "ChannelInternal.h"
#include "TimeSlotInternal.h"

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


qlonglong QueryExecutor::nextTimeSlotKey() const
{
  return nextKey("timeSlots_seq");
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


QSharedPointer<DataMark> QueryExecutor::insertNewTag(const QSharedPointer<DataMark>& tag)
{
  bool result;
  qlonglong newId = nextTagKey();

  syslog(LOG_INFO, "%s", QString("insertNewTag-start-").append(QString::number(newId)).toStdString().c_str());

  QSqlQuery newTagQuery(m_database);
  newTagQuery.prepare("insert into tag (altitude , latitude, longitude, label, description, url, user_id, time, id) "
    "         values(:altitude,:latitude,:longitude,:label,:description,:url,:user_id,:time,:id);");
  newTagQuery.bindValue(":altitude", tag->getAltitude());
  newTagQuery.bindValue(":latitude", tag->getLatitude());
  newTagQuery.bindValue(":longitude", tag->getLongitude());
  newTagQuery.bindValue(":label", tag->getLabel().isEmpty()? "New Mark" : tag->getLabel());
  newTagQuery.bindValue(":description", tag->getDescription());
  newTagQuery.bindValue(":url", tag->getUrl());
  newTagQuery.bindValue(":user_id", tag->getUser()->getId());
  newTagQuery.bindValue(":time", tag->getTime().toUTC());
  newTagQuery.bindValue(":id", newId);

  QSqlQuery putTagToChannelQuery(m_database);

  putTagToChannelQuery.prepare("insert into tags (tag_id,channel_id) values(:tag_id,:channel_id);");
  putTagToChannelQuery.bindValue(":tag_id",newId);
  putTagToChannelQuery.bindValue(":channel_id",tag->getChannel()->getId());

  m_database.transaction();

  result = newTagQuery.exec();
  if(!result)
  {
    m_database.rollback();
    syslog(LOG_INFO, "Rollback for NewTag sql query");
    return QSharedPointer<DataMark>(NULL);
  }

  result = putTagToChannelQuery.exec();
  if(!result)
  {
    m_database.rollback();
    syslog(LOG_INFO, "Rollback for putTagToChannel sql query");
    return QSharedPointer<DataMark>(NULL);
  }

  m_database.commit();

  QSharedPointer<DataMark> t(
    new DbDataMark(newId, tag->getAltitude(), tag->getLatitude(),
    tag->getLongitude(), tag->getLabel(),
    tag->getDescription(), tag->getUrl(),
    tag->getTime(), tag->getUser()->getId()));
  t->setUser(tag->getUser());
  t->setChannel(tag->getChannel());

  return t;
}


QSharedPointer<Channel> QueryExecutor::insertNewChannel(const QSharedPointer<Channel>& channel)
{
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


QSharedPointer<common::User> QueryExecutor::insertNewUser(const QSharedPointer<common::User>& user)
{
  bool result;
  QSqlQuery newUserQuery(m_database);
  qlonglong newId = nextUserKey();
  syslog(LOG_INFO,"Generating token for new user, %s : %s",user->getLogin().toStdString().c_str()
    ,user->getPassword().toStdString().c_str());
  QString newToken = generateNewToken(user->getLogin(),user->getPassword());
  //  syslog(LOG_INFO,"newToken = %s",newToken.toStdString().c_str());
  newUserQuery.prepare("insert into users (id,login,password,token) values(:id,:login,:password,:a_t);");
  newUserQuery.bindValue(":id",newId);
  syslog(LOG_INFO,"Sending: %s",newUserQuery.lastQuery().toStdString().c_str());
  newUserQuery.bindValue(":login",user->getLogin());
  syslog(LOG_INFO,"Sending: %s",newUserQuery.lastQuery().toStdString().c_str());
  newUserQuery.bindValue(":password",user->getPassword());
  syslog(LOG_INFO,"Sending: %s",newUserQuery.lastQuery().toStdString().c_str());
  newUserQuery.bindValue(":a_t",newToken);
  syslog(LOG_INFO,"Sending: %s",newUserQuery.lastQuery().toStdString().c_str());
  m_database.transaction();
  result=newUserQuery.exec();
  if(!result)
  {
    syslog(LOG_INFO,"Rollback for NewUser sql query");
    m_database.rollback();
    return QSharedPointer<common::User>(NULL);
  }else
  {
    syslog(LOG_INFO,"Commit for NewUser sql query");
    m_database.commit();
  }
  QSharedPointer<DbUser> newUser(new DbUser(user->getLogin(),user->getPassword(),newId,newToken));
  return newUser;
}


bool QueryExecutor::subscribeChannel(const QSharedPointer<common::User>& user,const QSharedPointer<Channel>& channel)
{
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


QSharedPointer<TimeSlot> QueryExecutor::insertNewTimeSlot(const QSharedPointer<TimeSlot>& timeSlot)
{
  bool result;
  QSqlQuery newTimeSlotQuery(m_database);
  qlonglong newId = nextTimeSlotKey();
  syslog(LOG_INFO,"NewId ready, now preparing sql query for adding new time slot");
  newTimeSlotQuery.prepare("insert into timeSlot (id,slot) values(:id,:slot);");
  newTimeSlotQuery.bindValue(":id",newId);
  newTimeSlotQuery.bindValue(":slot",timeSlot->getSlot());

  m_database.transaction();
  result=newTimeSlotQuery.exec();
  if(!result)
  {
    syslog(LOG_INFO,"Rollback for NewTimeSlot sql query");
    m_database.rollback();
    return QSharedPointer<TimeSlot>(NULL);
  } else
  {
    syslog(LOG_INFO,"Commit for NewTimeSlot sql query");
    m_database.commit();
  }
  QSharedPointer<DbTimeSlot> newTimeSlot(new DbTimeSlot(newId, timeSlot->getSlot()));
  return newTimeSlot;
}


bool QueryExecutor::insertNewChannelTimeSlot(const QSharedPointer<Channel>& channel, const QSharedPointer<TimeSlot>& timeSlot)
{
  bool result;
  QSqlQuery insertNewChannelTimeSlot(m_database);
  insertNewChannelTimeSlot.prepare("insert into channelTimeSlot (channel_id,timeSlot_id) values(:channel_id,:timeSlot_id);");
  insertNewChannelTimeSlot.bindValue(":channel_id",channel->getId());
  insertNewChannelTimeSlot.bindValue(":timeSlot_id",timeSlot->getId());

  syslog(LOG_INFO,"Inserting %llu millyseconds(Id = %lld) for %s (Id = %lld)", timeSlot->getSlot(),timeSlot->getId(),
    channel->getName().toStdString().c_str(), channel->getId());

  m_database.transaction();
  result=insertNewChannelTimeSlot.exec();
  if(!result)
  {
    syslog(LOG_INFO,"Rollback for insertNewChannelTimeSlot sql query");
    m_database.rollback();
  }
  else
  {
    syslog(LOG_INFO,"Commit for insertNewChannelTimeSlot sql query");
    m_database.commit();
  }
  return result;
}


bool QueryExecutor::changeChannelTimeSlot(const QSharedPointer<Channel>& channel, const QSharedPointer<TimeSlot>& timeSlot)
{
  bool result;
  QSqlQuery changeChannelTimeSlot(m_database);
  changeChannelTimeSlot.prepare("update channelTimeSlot set timeslot_id = :timeSlot_id where channel_id = :channel_id;");
  changeChannelTimeSlot.bindValue(":channel_id",channel->getId());
  changeChannelTimeSlot.bindValue(":timeSlot_id",timeSlot->getId());

  //syslog(LOG_INFO,"Set %llu millyseconds(Id = %lld) for %s (Id = %lld)", timeSlot->getSlot(),timeSlot->getId(),
  //   channel->getName().toStdString().c_str(), channel->getId());
  m_database.transaction();
  result=changeChannelTimeSlot.exec();
  if(!result)
  {
    syslog(LOG_INFO,"Rollback for changeChannelTimeSlot sql query");
    m_database.rollback();
  }else
  {
    syslog(LOG_INFO,"Commit for changeChannelTimeSlot sql query");
    m_database.commit();
  }
  return result;
}


bool QueryExecutor::insertNewMarkTimeSlot(const QSharedPointer<DataMark>& tag, const QSharedPointer<TimeSlot>& timeSlot)
{
  bool result;
  QSqlQuery insertNewMarkTimeSlot(m_database);
  insertNewMarkTimeSlot.prepare("insert into tagTimeSlot (tag_id,timeSlot_id) values(:tag_id,:timeSlot_id);");
  insertNewMarkTimeSlot.bindValue(":tag_id",tag->getId());
  insertNewMarkTimeSlot.bindValue(":timeSlot_id",timeSlot->getId());

  syslog(LOG_INFO,"Inserting %llu millyseconds(Id = %lld) for tag (Id = %lld)", timeSlot->getSlot(),timeSlot->getId(),
    tag->getId());

  m_database.transaction();
  result=insertNewMarkTimeSlot.exec();
  if(!result)
  {
    syslog(LOG_INFO,"Rollback for insertNewMarkTimeSlot sql query");
    m_database.rollback();
  }
  else
  {
    syslog(LOG_INFO,"Commit for insertNewMarkTimeSlot sql query");
    m_database.commit();
  }
  return result;
}


bool QueryExecutor::changeMarkTimeSlot(const QSharedPointer<DataMark>& tag, const QSharedPointer<TimeSlot>& timeSlot)
{
  bool result;
  QSqlQuery changeMarkTimeSlot(m_database);
  changeMarkTimeSlot.prepare("update tagTimeSlot set timeslot_id = :timeSlot_id where tag_id = :tag_id;");
  changeMarkTimeSlot.bindValue(":tag_id",tag->getId());
  changeMarkTimeSlot.bindValue(":timeSlot_id",timeSlot->getId());

  syslog(LOG_INFO,"Set %llu millyseconds(Id = %lld) for tag (Id = %lld)", timeSlot->getSlot(),timeSlot->getId(), tag->getId());

  m_database.transaction();
  result=changeMarkTimeSlot.exec();
  if(!result)
  {
    syslog(LOG_INFO,"Rollback for changeMarkTimeSlot sql query");
    m_database.rollback();
  }else
  {
    syslog(LOG_INFO,"Commit for changeMarkTimeSlot sql query");
    m_database.commit();
  }
  return result;
}


bool QueryExecutor::deleteChannelTimeSlot(const QSharedPointer<Channel>& channel)
{
  bool result;
  QSqlQuery deleteChannelTimeSlot(m_database);
  deleteChannelTimeSlot.prepare("delete from channelTimeSlot where channel_id = :channel_id;");
  deleteChannelTimeSlot.bindValue(":channel_id",channel->getId());

  syslog(LOG_INFO,"Deleting channel %s (Id = %lld)", channel->getName().toStdString().c_str(), channel->getId());

  m_database.transaction();
  result=deleteChannelTimeSlot.exec();
  if(!result)
  {
    syslog(LOG_INFO,"Rollback for deleteChannelTimeSlot sql query");
    m_database.rollback();
  }
  else
  {
    syslog(LOG_INFO,"Commit for deleteChannelTimeSlot sql query");
    m_database.commit();
  }
  return result;
}


bool QueryExecutor::deleteMarkTimeSlot(const QSharedPointer<DataMark>& tag)
{
  bool result;
  QSqlQuery deleteMarkTimeSlot(m_database);
  deleteMarkTimeSlot.prepare("delete from tagTimeSlot where tag_id = :tag_id;");
  deleteMarkTimeSlot.bindValue(":tag_id",tag->getId());

  syslog(LOG_INFO,"Deleting tag (Id = %lld)", tag->getId());

  m_database.transaction();
  result=deleteMarkTimeSlot.exec();
  if(!result)
  {
    syslog(LOG_INFO,"Rollback for deleteMarkTimeSlot sql query");
    m_database.rollback();
  }
  else
  {
    syslog(LOG_INFO,"Commit for deleteMarkTimeSlot sql query");
    m_database.commit();
  }
  return result;
}


bool QueryExecutor::unsubscribeChannel(const QSharedPointer<common::User>& user,const QSharedPointer<Channel>& channel)
{
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
