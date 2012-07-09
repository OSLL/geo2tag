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

#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <QByteArray>
#include <QObject>
#include <QVariantMap>
#include "common/inc/DataMarks.h"
#include "common/inc/Channel.h"
#include "common/inc/User.h"
#include "common/inc/Session.h"

class JsonSerializer:public QObject
{

  protected:
    QSharedPointer<Channels>    m_channelsContainer;
    QSharedPointer<DataMarks>   m_tagsContainer;
    QSharedPointer<common::Users>       m_usersContainer;
    QSharedPointer<Sessions>    m_sessionsContainer;

    QVariantMap m_jsonTree;

    int m_errno;

    QString m_status;
    QString m_statusMessage;

    void clearContainers();
  public:
    JsonSerializer(QObject * parent=0);

    void addChannel(const QSharedPointer<Channel>&);
    void addTag(const QSharedPointer<DataMark>&);
    void addUser(const QSharedPointer<common::User>&);
    void addSession(const QSharedPointer<Session>&);

    virtual QByteArray getJson() const = 0;

    virtual bool parseJson(const QByteArray&) = 0;

    QSharedPointer<DataMarks> getTags() const;
    QSharedPointer<common::Users> getUsers() const;
    QSharedPointer<Channels> getChannels() const;
    QSharedPointer<Sessions> getSessions() const;

    const QString& getStatus() const;
    void setStatus(const QString&);

    const QString& getStatusMessage() const;
    void setStatusMessage(const QString&);

    const int& getErrno() const;
    void setErrno(const int&);

    ~JsonSerializer();
};
// JSONSERIALIZER_H
#endif
