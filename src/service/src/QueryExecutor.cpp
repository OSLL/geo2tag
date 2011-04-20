#include <syslog.h>
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
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
    QSqlQuery newTagQuery(m_database);
    qlonglong newId = nextTagKey();
    newTagQuery.prepare("insert into tag (latitude, longitude, label, description, url, user_id, time, id) "
                        "         values(:latitude,:longitude,:label,:description,:url,:user_id,:time,:id);");
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
        return QSharedPointer<DataMark>(NULL);
    }
    result = putTagToChannelQuery.exec();
    if(!result)
    {
        m_database.rollback();
        return QSharedPointer<DataMark>(NULL);
    }

    m_database.commit();

    QSharedPointer<DataMark> t(
            new DbDataMark(newId,tag->getLatitude(),tag->getLongitude(),tag->getLabel(),
                           tag->getDescription(),tag->getUrl(),tag->getTime(),tag->getUser()->getId()));
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

    QSqlQuery putTimeSlotToChannelQuery(m_database);
    putTimeSlotToChannelQuery.prepare("insert into channeltimeslot (channel_id,timeSlot_id) values(:channel_id,:timeSlot_id);");
    putTimeSlotToChannelQuery.bindValue(":channel_id", newId);
    putTimeSlotToChannelQuery.bindValue(":timeSlot_id", 1); //NOTE: First value in table "timeslot" must be 31536000000 millyseconds

    syslog(LOG_INFO,"Inserting default value = 31536000000 millyseconds = 1 year for %s (Id = %lld)", channel->getName().toStdString().c_str(), channel->getId());

    m_database.transaction();

    result=newChannelQuery.exec();
    if(!result)
    {
      syslog(LOG_INFO,"Rollback for NewChannel sql query");
      m_database.rollback();
      return QSharedPointer<Channel>(NULL);
    }else     
      syslog(LOG_INFO,"Commit for NewChannel sql query - insert in table channel");

    result=putTimeSlotToChannelQuery.exec();
    if(!result)
    {
      syslog(LOG_INFO,"Rollback for NewChannel sql query");
      m_database.rollback();
      return QSharedPointer<Channel>(NULL);
    }else
      syslog(LOG_INFO,"Commit for NewChannel sql query - - insert in table channeltimeslot");

    m_database.commit();


    QSharedPointer<DbChannel> newChannel(new DbChannel(newId,channel->getName(),channel->getDescription(),channel->getUrl()));
    return newChannel;
}

QSharedPointer<User> QueryExecutor::insertNewUser(const QSharedPointer<User>& user)
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
			return QSharedPointer<User>(NULL);
    }else 
    {
      syslog(LOG_INFO,"Commit for NewUser sql query");
      m_database.commit();
    }
		QSharedPointer<DbUser> newUser(new DbUser(user->getLogin(),user->getPassword(),newId,newToken));
    return newUser;
}

bool QueryExecutor::subscribeChannel(const QSharedPointer<User>& user,const QSharedPointer<Channel>& channel)
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

bool QueryExecutor::changeChannelTimeSlot(const QSharedPointer<Channel>& channel, const QSharedPointer<TimeSlot>& timeSlot)
{
    syslog(LOG_INFO, "test6");
    bool result;
    QSqlQuery changeChannelTimeSlot(m_database);
    changeChannelTimeSlot.prepare("update channelTimeSlot set timeslot_id = :timeSlot_id where channel_id = :channel_id;");
    changeChannelTimeSlot.bindValue(":channel_id",channel->getId());
    changeChannelTimeSlot.bindValue(":timeSlot_id",timeSlot->getId());

    //syslog(LOG_INFO,"Set %llu millyseconds(Id = %lld) for %s (Id = %lld)", timeSlot->getSlot(),timeSlot->getId(),
                             //   channel->getName().toStdString().c_str(), channel->getId());
syslog(LOG_INFO, "test7");
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
