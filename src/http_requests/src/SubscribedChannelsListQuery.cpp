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

/*! ---------------------------------------------------------------
 * $Id$ 
 *
 * \file SubscribedChannelsListQuery.cpp
 * \brief SubscribedChannelsListQuery implementation
 *
 * File description
 *
 * PROJ: geo2tag
 * ---------------------------------------------------------------- */
#include <sstream>
#include "SubscribedChannelsListQuery.h"
#include "defines.h"
#include <QDebug>
#include "ChannelListJSON.h"

namespace GUI
{
    SubscribedChannelsListQuery::SubscribedChannelsListQuery(QObject *parent)
        : QObject(parent)
    {
        jsonQuery = "";
        httpQuery = "";
        manager = new QNetworkAccessManager(this);

        qDebug() << "Free SubscribedChannelsListQuery created";
    }

    SubscribedChannelsListQuery::SubscribedChannelsListQuery(QString auth_token, QObject *parent)
                : QObject(parent)
    {
        manager = new QNetworkAccessManager(this);
        setQuery(auth_token);
        qDebug() << "SubscribedChannelsListQuery created:\n"
                 << httpQuery << jsonQuery;
    }

    void SubscribedChannelsListQuery::setQuery(QString auth_token)
    {
        jsonQuery = "{\"auth_token\":\"" + auth_token + "\"}";
        httpQuery = SUBSCRIBED_LIST_HTTP_URL;
    }

    SubscribedChannelsListQuery::~SubscribedChannelsListQuery()
    {

    }

    const QString& SubscribedChannelsListQuery::getHttpQuery()
    {
        return httpQuery;
    }

    const QString& SubscribedChannelsListQuery::getJsonQuery()
    {
        return jsonQuery;
    }

    void SubscribedChannelsListQuery::doRequest()
    {
        if (httpQuery == "" || jsonQuery == "")
        {
            qDebug() << "SubscribedChannelsListQuery: can't do request cause query isn't set";
            return;
        }

        QNetworkRequest request;
        request.setUrl(QUrl(httpQuery));

        QByteArray data(jsonQuery.toAscii(), jsonQuery.size());

        QNetworkReply *reply = manager->post(request, data);

        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(onManagerFinished(QNetworkReply*)));
        connect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                this, SLOT(onManagerSslErrors()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(onReplyError(QNetworkReply::NetworkError)));

        qDebug() << "SubscribedChannelsListQuery did request:\n"
                 << httpQuery << jsonQuery;
    }

    void SubscribedChannelsListQuery::onManagerFinished(QNetworkReply *reply)
    {
        QByteArray jsonResponseByteArray = reply->readAll();

        if (jsonResponseByteArray.size() > 0)
        {
            QString jsonResponse(jsonResponseByteArray);
            qDebug() << "Gotten response (json): " << jsonResponse;
            std::stringstream jsonStream(jsonResponse.toStdString());
            ChannelListResponseJSON channelList(jsonStream);
            CHandlePtr<common::Channels> channels = channelList.getChannels();
            emit responseReceived(channels);
        }
    }

    void SubscribedChannelsListQuery::onReplyError(QNetworkReply::NetworkError error)
    {
        qDebug("Network error: %d \n", error);
    }

    void SubscribedChannelsListQuery::onManagerSslErrors()
    {
        qDebug("ssl error \n");
    }
} // namespace GUI

/* ===[ End of file $HeadURL$ ]=== */
