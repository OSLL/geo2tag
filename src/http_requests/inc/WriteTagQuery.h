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

#ifndef WRITETAGQUERY_H
#define WRITETAGQUERY_H

#include <QObject>
#include <QString>
#include "DefaultQuery.h"
#include "User.h"
#include "Session.h"
#include "Channel.h"
#include "DataMarks.h"

class WriteTagQuery: public DefaultQuery
{
  Q_OBJECT

    QSharedPointer<Session> m_session;
    QSharedPointer<Channel> m_channel;
    QSharedPointer<DataMark> m_tag;

  virtual QString getUrl() const;
  virtual QByteArray getRequestBody() const;
  virtual void processReply(QNetworkReply *reply);

  public:
    WriteTagQuery(const QSharedPointer<Session>& session,
                  const QSharedPointer<Channel>& channel,
                  const QSharedPointer<DataMark>& tag, QObject *parent = 0);
    WriteTagQuery(QObject *parent = 0);

    ~WriteTagQuery();
    void setTag(const QSharedPointer<DataMark> &tag);
    QSharedPointer<DataMark> getTag();
    const QSharedPointer<DataMark>& getTag() const;

    void setSession(const QSharedPointer<Session>& session);
    const QSharedPointer<Session>& getSession() const;

    void setChannel(const QSharedPointer<Channel>& channel);
    const QSharedPointer<Channel>& getChannel() const;

    Q_SIGNALS:

    void tagAdded();
};
// ADDNEWMARKQUERY_H
#endif
