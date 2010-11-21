#include <syslog.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include "QueryExecutor.h"

#include "DataMarkInternal.h"
#include "UserInternal.h"
#include "ChannelInternal.h"

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


qlonglong QueryExecutor::nextUserKey() const
{
    return nextKey("tags_seq");
}

QSharedPointer<DataMark> QueryExecutor::insertNewTag(const QSharedPointer<DataMark>& tag)
{
    bool result;
    QSqlQuery newTagQuery(m_database);
    qlonglong newId = nextUserKey();
    newTagQuery.prepare("insert into tag (latitude, longitude, label, description, url, user_id, time, id) "
                        "         values(:latitude,:longitude,:label,:description,:url,:user_id,:time,:id);");
    newTagQuery.bindValue(":latitude", tag->getLatitude());
    newTagQuery.bindValue(":longitude", tag->getLongitude());
    newTagQuery.bindValue(":label", tag->getLabel());
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
