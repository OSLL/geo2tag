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

#include <QtSql>
#include <QMap>

namespace common
{
    DbObjectsCollection::DbObjectsCollection():
            m_channelsContainer(new Channels()),
            m_tagsContainer(new DataMarks()),
            m_usersContainer(new Users()),
            m_updateThread(m_tagsContainer, m_usersContainer, m_channelsContainer, NULL)
    {
        m_updateThread.start();
    }

    DbObjectsCollection& DbObjectsCollection::getInstance()
    {
        static DbObjectsCollection s;
        return s;
    }

    DbObjectsCollection::~DbObjectsCollection()
    {
        m_updateThread.wait();
    }

    QByteArray DbObjectsCollection::process(const QString& queryType, const QByteArray& body)
    {
        QByteArray answer;
        if(queryType == "login")
        {
            LoginRequestJSON request;
            request.parseJson(body);

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
                        //wrong password
                    }
                }
            }
            answer.append("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
            if(realUser.isNull())
            {

            }
            else
            {
                syslog(LOG_INFO, "found token: %s", realUser->getToken().toStdString().c_str());
                LoginResponseJSON response;
                response.addUser(realUser);
                answer.append(response.getJson());
            }
            return answer;
        }
    }
} // namespace common

/* ===[ End of file ]=== */
