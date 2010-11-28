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
#include "DbSession.h"

#include "LoginRequestJSON.h"
#include "LoginResponseJSON.h"

#include "AddNewMarkRequestJSON.h"
#include "AddNewMarkResponseJSON.h"

#include "RSSFeedRequestJSON.h"
#include "RSSFeedJSON.h"

#include <QtSql>
#include <QMap>

namespace common
{
    DbObjectsCollection::DbObjectsCollection():
            m_channelsContainer(new Channels()),
            m_tagsContainer(new DataMarks()),
            m_usersContainer(new Users()),
            m_dataChannelsMap(new DataChannels()),
            m_updateThread(NULL)
    {

        m_processors.insert("login", &DbObjectsCollection::processLoginQuery);
        m_processors.insert("apply", &DbObjectsCollection::processAddNewMarkQuery);
        m_processors.insert("rss", &DbObjectsCollection::processRssFeedQuery);

        QSqlDatabase database = QSqlDatabase::addDatabase("QPSQL");
        database.setHostName("localhost");
        database.setDatabaseName("geo2tag");
        database.setUserName("geo2tag");
        database.setPassword("");

        m_updateThread = new UpdateThread(
                        QSqlDatabase::cloneDatabase(database,"updateThread"),
                        m_tagsContainer,
                        m_usersContainer,
                        m_channelsContainer,
                        m_dataChannelsMap,
                        NULL);

        m_updateThread->start();

        m_queryExecutor = new QueryExecutor(QSqlDatabase::cloneDatabase(database,"executor"), NULL);

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

        ProcessMethod method = m_processors.value(queryType);
        return (*this.*method)(body);
    }

    QSharedPointer<User> DbObjectsCollection::findUserFromToken(const QSharedPointer<User> &dummyUser) const
    {
        QSharedPointer<User> realUser; // Null pointer
        QVector<QSharedPointer<User> > currentUsers = m_usersContainer->vector();
        syslog(LOG_INFO, "checking user's key: %s from %d known users", dummyUser->getToken().toStdString().c_str(),
                                currentUsers.size());
        for(int i=0; i<currentUsers.size(); i++)
        {
            if(currentUsers.at(i)->getToken() == dummyUser->getToken())
            {
                realUser = currentUsers.at(i);
                break;
            }
        }
        return realUser;
    }

    QByteArray DbObjectsCollection::processLoginQuery(const QByteArray &data)
    {
        LoginRequestJSON request;
        LoginResponseJSON response;
        QByteArray answer;

        request.parseJson(data);

        QSharedPointer<User> dummyUser = request.getUsers()->at(0);
        QSharedPointer<User> realUser; // Null pointer
        QVector<QSharedPointer<User> > currentUsers = m_usersContainer->vector();

        for(int i=0; i<currentUsers.size(); i++)
        {
            if(currentUsers.at(i)->getLogin() == dummyUser->getLogin())
            {
                if(currentUsers.at(i)->getPassword() == dummyUser->getPassword())
                {
                    realUser = currentUsers.at(i);
                    break;
                }
                else
                {
                    response.setStatus("Error");
                    response.setStatusMessage("Wrong password");
                }
            }
        }
        answer.append("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
        if(realUser.isNull())
        {
            response.setStatus("Error");
            response.setStatusMessage("Wrong login or password");
        }
        else
        {
            response.setStatus("Ok");
            response.setStatusMessage("Authorization was successful");
            response.addUser(realUser);
        }

        answer.append(response.getJson());
        syslog(LOG_INFO, "answer: %s", answer.data());
        return answer;
    }

    QByteArray DbObjectsCollection::processAddNewMarkQuery(const QByteArray &data)
    {
        AddNewMarkRequestJSON request;
        AddNewMarkResponseJSON response;
        QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

        request.parseJson(data);
        QSharedPointer<DataMark> dummyTag = request.getTags()->at(0);
        QSharedPointer<User> dummyUser = dummyTag->getUser();
        QSharedPointer<User> realUser = findUserFromToken(dummyUser);

        if(realUser.isNull())
        {
            response.setStatus("Error");
            response.setStatusMessage("Wrong authentification key");
            answer.append(response.getJson());
            return answer;
        }

        QSharedPointer<Channel> dummyChannel = dummyTag->getChannel();
        QSharedPointer<Channel> realChannel; // Null pointer
        QVector<QSharedPointer<Channel> > currentChannels = m_channelsContainer->vector();

        for(int i=0; i<currentChannels.size(); i++)
        {
            if(currentChannels.at(i)->getName() == dummyChannel->getName())
            {
                realChannel = currentChannels.at(i);
            }
        }
        if(realChannel.isNull())
        {
            response.setStatus("Error");
            response.setStatusMessage("Wrong channel's' name");
            answer.append(response.getJson());
            return answer;
        }

        dummyTag->setChannel(realChannel);
        dummyTag->setUser(realUser);
        QSharedPointer<DataMark> realTag = m_queryExecutor->insertNewTag(dummyTag);
        if(realTag ==NULL)
        {
            response.setStatus("Error");
            response.setStatusMessage("Internal server error ):");
            answer.append(response.getJson());
            return answer;
        }
        m_updateThread->lockWriting();
        m_tagsContainer->push_back(realTag);
        m_updateThread->unlockWriting();

        response.setStatus("Ok");
        response.setStatusMessage("Tag has been added");
        answer.append(response.getJson());
        syslog(LOG_INFO, "answer: %s", answer.data());
        return answer;
    }


    QByteArray DbObjectsCollection::processRssFeedQuery(const QByteArray &data)
    {
        RSSFeedRequestJSON request;
        QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

        request.parseJson(data);
        QSharedPointer<User> dummyUser = request.getUsers()->at(0);
        QSharedPointer<User> realUser = findUserFromToken(dummyUser);
        if(realUser.isNull())
        {
            RSSFeedResponseJSON response;
            response.setStatus("Error");
            response.setStatusMessage("Wrong authentification key");
            answer.append(response.getJson());
            return answer;
        }

        QSharedPointer<Channels> channels = realUser->getSubscribedChannels();
        DataChannels feed;
//        syslog(LOG_INFO, "rssfeed processing: user %s has %d channels subscribed",
//               realUser->getLogin().toStdString().c_str(), channels->size());
        for(int i = 0; i<channels->size(); i++)
        {
            QSharedPointer<Channel> channel = channels->at(i);
            QList<QSharedPointer<DataMark> > tags = m_dataChannelsMap->values(channel);
            qSort(tags);
            QList<QSharedPointer<DataMark> > last10 = tags.mid(tags.size()>10?tags.size()-10:0, 10);
            for(int j = 0; j<last10.size(); j++)
            {
//                syslog(LOG_INFO,"rssfeed: adding tag with time: %s", last10.at(j)->getTime().toString("dd MM yyyy HH:mm:ss.zzz").toStdString().c_str());
                feed.insert(channel, last10.at(j));
            }
        }
        RSSFeedResponseJSON response(feed);
        response.setStatus("Ok");
        response.setStatusMessage("feed has been generated");
        answer.append(response.getJson());
        syslog(LOG_INFO, "answer: %s", answer.data());
        return answer;
    }
} // namespace common

/* ===[ End of file ]=== */
