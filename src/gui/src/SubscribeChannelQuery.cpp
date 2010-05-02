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
 * \file SubscribeChannelQuery.cpp
 * \brief SubscribeChannelQuery implementation
 *
 * File description
 *
 * PROJ: geo2tag
 * ---------------------------------------------------------------- */

#include "defines.h"
#include "SubscribeChannelQuery.h"
#include "SubscribeChannelJSON.h"
#include <QDebug>

namespace GUI
{
    SubscribeChannelQuery::SubscribeChannelQuery(QObject *parent)
        : QObject(parent)
    {
        jsonQuery = "";
        httpQuery = "";
        manager = new QNetworkAccessManager(this);

        qDebug() << "Free SubscribeChannelQuery created";
    }

    SubscribeChannelQuery::SubscribeChannelQuery(QString auth_token, QString channel, QObject *parent)
                : QObject(parent)
    {
        manager = new QNetworkAccessManager(this);
        setQuery(auth_token, channel);
        qDebug() << "SubscribeChannelQuery created:\n"
                 << httpQuery << jsonQuery;
    }

    void SubscribeChannelQuery::setQuery(QString auth_token, QString channel)
    {
        jsonQuery = SubscribeChannelJSON::convertToJSON(auth_token, channel);
        httpQuery = SUBSCRIBE_HTTP_URL;
    }

    SubscribeChannelQuery::~SubscribeChannelQuery()
    {

    }

    const QString& SubscribeChannelQuery::getHttpQuery()
    {
        return httpQuery;
    }

    const QString& SubscribeChannelQuery::getJsonQuery()
    {
        return jsonQuery;
    }

    void SubscribeChannelQuery::doRequest()
    {
        if (httpQuery == "" || jsonQuery == "")
        {
            qDebug() << "SubscribeChannelQuery: can't do request because query isn't set";
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

        qDebug() << "SubscribeChannelQuery did request:\n"
                 << httpQuery << jsonQuery;
    }

    void SubscribeChannelQuery::onManagerFinished(QNetworkReply *reply)
    {
        QByteArray jsonResponseByteArray = reply->readAll();

        if (jsonResponseByteArray.size() > 0)
        {
            QString jsonResponse(jsonResponseByteArray);
            qDebug() << "Gotten response (json): " << jsonResponse;
            QString status = SubscribeChannelJSON::convertToSatus(jsonResponse);
            emit responseReceived(status);
        }
    }

    void SubscribeChannelQuery::onReplyError(QNetworkReply::NetworkError error)
    {
        qDebug("Network error: %d \n", error);
    }

    void SubscribeChannelQuery::onManagerSslErrors()
    {
        qDebug("ssl error \n");
    }
} // namespace GUI

/* ===[ End of file $HeadURL$ ]=== */
