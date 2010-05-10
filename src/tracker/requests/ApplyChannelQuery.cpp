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
 * \file ApplyChannelQuery.cpp
 * \brief ApplyChannelQuery implementation
 *
 * File description
 *
 * PROJ: OSLL/fedexday
 * ---------------------------------------------------------------- */

#include "ApplyChannelQuery.h"
#include "defines.h"
#include <QDebug>
#include "qjson/parser.h"
#include "qjson/serializer.h"
#include "QVariant"
#include "QVariantMap"

#ifndef APPLY_CHANNEL_HTTP_URL
#define APPLY_CHANNEL_HTTP_URL "http://zps.spb.su/example?query=addChannel"
#endif

ApplyChannelQuery::ApplyChannelQuery(QObject *parent)
    : QObject(parent)
{
    jsonQuery = "";
    httpQuery = "";
    manager = new QNetworkAccessManager(this);

    qDebug() << "Free ApplyChannelQuery created";
}

ApplyChannelQuery::ApplyChannelQuery(QString auth_token, QString name, QString description,
                                     QString url, qreal activeRadius, QObject *parent)
            : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    setQuery(auth_token, name, description, url, activeRadius);
    qDebug() << "ApplyChannelQuery created:\n"
             << httpQuery << jsonQuery;
}

void ApplyChannelQuery::setQuery(QString auth_token, QString name, QString description,
                                 QString url, qreal activeRadius)
{
    QVariantMap request;
    request.insert("auth_token", auth_token);
    request.insert("name", name);
    request.insert("description", description);
    request.insert("url", url);
    request.insert("activeRadius", activeRadius);
    QJson::Serializer serializer;
    jsonQuery = serializer.serialize(request);

    httpQuery = APPLY_CHANNEL_HTTP_URL;
}

ApplyChannelQuery::~ApplyChannelQuery()
{

}

const QString& ApplyChannelQuery::getHttpQuery()
{
    return httpQuery;
}

const QString& ApplyChannelQuery::getJsonQuery()
{
    return jsonQuery;
}

void ApplyChannelQuery::doRequest()
{
    if (httpQuery == "" || jsonQuery == "")
    {
        qDebug() << "ApplyChannelQuery: can't do request because query isn't set";
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

    qDebug() << "ApplyChannelQuery did request:\n"
             << httpQuery << jsonQuery;
}

void ApplyChannelQuery::onManagerFinished(QNetworkReply *reply)
{
    QByteArray jsonResponseByteArray = reply->readAll();

    if (jsonResponseByteArray.size() > 0)
    {
        QString jsonResponse(jsonResponseByteArray);
        qDebug() << "Gotten response (json): " << jsonResponse;
        QJson::Parser parser;
        bool ok;
        QVariantMap result = parser.parse(QByteArray(jsonResponse.toAscii()), &ok).toMap();
        QString status("");
        if (!ok)
        {
            qFatal("An error occured during parsing json with response to apply mark");
        }
        else
        {
            status = result["status"].toString();
        }

        emit responseReceived(status);
    }
}

void ApplyChannelQuery::onReplyError(QNetworkReply::NetworkError error)
{
    qDebug("Network error: %d \n", error);
}

void ApplyChannelQuery::onManagerSslErrors()
{
    qDebug("ssl error \n");
}



/* ===[ End of file $HeadURL$ ]=== */
