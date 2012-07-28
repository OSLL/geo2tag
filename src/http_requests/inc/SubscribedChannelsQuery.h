/*
 * Copyright 2010  Open Source & Linux Lab (OSLL)  osll@osll.spb.ru
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

/* $Id$ */
/*!
 * \file SubscribedChannelsQuery.h
 * \brief Header of SubscribedChannelsQuery
 * \todo add comment here
 *
 * File description
 *
 * PROJ: geo2tag
 * ---------------------------------------------------------------- */

#ifndef _SubscribedChannelsQuery_H_714B1547_7B50_4397_B7A4_1964DBF03673_INCLUDED_
#define _SubscribedChannelsQuery_H_714B1547_7B50_4397_B7A4_1964DBF03673_INCLUDED_

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "DefaultQuery.h"
#include "Channel.h"
#include "Session.h"

/*!
 * SubscribedChannelsQuery class definition.
 *
 * The object of this class represents http query to server.
 * This query includes json request for list of subscribed channels.
 *
 */
class SubscribedChannelsQuery : public DefaultQuery
{
  Q_OBJECT

    QSharedPointer<Session> m_session;
  QSharedPointer<Channels> m_channels;

  virtual QString getUrl() const;
  virtual QByteArray getRequestBody() const;

  private Q_SLOTS:

    virtual void processResponse(const QByteArray &data);

  public:

    SubscribedChannelsQuery(QObject *parent = 0);

    SubscribedChannelsQuery(const QSharedPointer<Session>& session, QObject *parent = 0);

    ~SubscribedChannelsQuery();

    const QSharedPointer<Channels>& getChannels() const;
    void setQuery(const QSharedPointer<Session>& session);

    Q_SIGNALS:

    // class SubscribedChannelsQuery
};

//_SubscribedChannelsQuery_H_714B1547_7B50_4397_B7A4_1964DBF03673_INCLUDED_
#endif

/* ===[ End of file $HeadURL$ ]=== */
