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
 * \file RSSFeedQuery.cpp
 * \brief RSSFeedQuery implementation
 *
 * File description
 *
 * PROJ: geo2tag
 * ---------------------------------------------------------------- */

#include "RSSFeedQuery.h"
#include "defines.h"
#include <QDebug>
#include "RSSFeedJSON.h"
#include "RSSFeedRequestJSON.h"
#include "JsonDataMark.h"
#include "JsonUser.h"

RSSFeedQuery::RSSFeedQuery(QString auth_token, qreal latitude, qreal longitude,
                           qreal radius, QString type, QObject *parent):
        DefaultQuery(parent), m_auth_token(auth_token), m_latitude(latitude),
        m_longitude(longitude), m_radius(radius), m_type(type)
{
}

QString RSSFeedQuery::getUrl() const
{
    return FEED_HTTP_URL;
}

QByteArray RSSFeedQuery::getRequestBody() const
{
    RSSFeedRequestJSON request;
    request.setAuthToken(m_auth_token);
    request.setLatitude(m_latitude);
    request.setLongitude(m_longitude);
    request.setRadius(m_radius);
    request.setType(m_type);

    return request.getJson();
}

void RSSFeedQuery::processReply(QNetworkReply *reply)
{
    RSSFeedResponseJSON response;
    response.parseJson(reply->readAll());
    if(response.getStatus() == "Ok")
    {
        QSharedPointer<DataMarks> marks = response.getTags();
        QSharedPointer<DataMarks> newMarks(new DataMarks());

        int size = marks->size();
        for (int i = 0; i < size; i++)
        {
            QString title = marks->at(i)->getLabel();
            QString link = marks->at(i)->getUrl();
            QString description = marks->at(i)->getDescription();
            double latitude = marks->at(i)->getLatitude();
            double longitude = marks->at(i)->getLongitude();
            QDateTime time = marks->at(i)->getTime();

            QSharedPointer<User> user(new JsonUser(marks->at(i)->getUser()->getLogin()));

            QSharedPointer<JsonDataMark> newMark(new JsonDataMark(latitude,
                                                     longitude,
                                                     title,
                                                     description,
                                                     link,
                                                     time));
            newMark->setUser(user);
            newMarks->push_back(newMark);
        }

        m_marks = newMarks;
        emit connected();
    }
    else
    {
        emit errorOccured(response.getStatusMessage());
    }

}

QSharedPointer<DataMarks> RSSFeedQuery::getMarks() const
{
    return m_marks;
}

RSSFeedQuery::~RSSFeedQuery()
{

}

/* ===[ End of file $HeadURL$ ]=== */
