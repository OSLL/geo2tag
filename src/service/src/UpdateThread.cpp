#include <syslog.h>
#include <QDebug>
#include "UpdateThread.h"

UpdateThread::UpdateThread(const QSqlDatabase &db,
                           const QSharedPointer<DataMarks> &tags,
                           const QSharedPointer<Users> &users,
                           const QSharedPointer<Channels> &channels,
                           const QSharedPointer<DataChannels>& dataChannelsMap,
                           QObject *parent):
    QThread(parent),
    m_channelsContainer(channels),
    m_tagsContainer(tags),
    m_usersContainer(users),
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
        Users       usersContainer(*m_usersContainer);
        DataMarks   tagsContainer(*m_tagsContainer);
        Channels    channelsContainer(*m_channelsContainer);

        loadUsers(usersContainer);
        loadTags(tagsContainer);
        loadChannels(channelsContainer);

        lockWriting();
        m_usersContainer->merge(usersContainer);
        m_tagsContainer->merge(tagsContainer);
        m_channelsContainer->merge(channelsContainer);

        updateReflections(*m_tagsContainer,*m_usersContainer, *m_channelsContainer);

        for(int i=0; i<m_tagsContainer->size(); i++)
        {
            QSharedPointer<DataMark> tag = m_tagsContainer->at(i);
            QSharedPointer<Channel> channel = tag->getChannel();
            if(!m_dataChannelsMap->contains(channel, tag))
            {
                syslog(LOG_INFO, "adding tag %s to channel %s",
                       tag->getTime().toString("dd MM yyyy HH:mm:ss.zzz").toStdString().c_str(),
                       channel->getName().toStdString().c_str());
                m_dataChannelsMap->insert(channel, tag);
            }
        }
        unlockWriting();

        syslog(LOG_INFO, "current users' size = %d",m_usersContainer->size());
        syslog(LOG_INFO, "current tags' size = %d",m_tagsContainer->size());
        syslog(LOG_INFO,  "current channels' size = %d", m_channelsContainer->size());
        m_database.close();
        QThread::msleep(10000);
    }
}

void UpdateThread::loadUsers(Users &container)
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

void UpdateThread::loadTags(DataMarks &container)
{
    QSqlQuery query(m_database);
    query.exec("select id, time, latitude, longitude, label, description, url, user_id from tag order by time;");
    while (query.next())
    {
        qlonglong id = query.record().value("id").toLongLong();
        if(container.exist(id))
        {
            // skip record
            continue;
        }
        QDateTime time = query.record().value("time").toDateTime().toTimeSpec(Qt::LocalTime);
//        syslog(LOG_INFO, "loaded tag with time: %s", time.toString("dd MM yyyy HH:mm:ss.zzz").toStdString().c_str());
        qreal latitude = query.record().value("latitude").toReal();
        qreal longitude = query.record().value("longitude").toReal();
        QString label = query.record().value("label").toString();
        QString description = query.record().value("description").toString();
        QString url = query.record().value("url").toString();
        qlonglong userId = query.record().value("user_id").toLongLong();

        DbDataMark *newMark = new DbDataMark(id,
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

void UpdateThread::updateReflections(DataMarks &tags, Users &users, Channels &channels)
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

}
