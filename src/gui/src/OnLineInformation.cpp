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
 * $Id$ 
 *
 * \file OnLineInformation.cpp
 * \brief OnLineInformation implementation
 *
 * File description
 *
 * PROJ: geo2tag
 * ---------------------------------------------------------------- */

#include "OnLineInformation.h"

#include <QDebug>
#include <QDateTime>
#include <DataMarks.h>
#include <QTimer>

#include "GpsInfo.h"

#ifndef DEFAULT_RADIUS
#define DEFAULT_RADIUS 30
#endif

#ifndef DEFAULT_USER
#define DEFAULT_USER "Paul"
#endif

namespace GUI
{
    OnLineInformation::OnLineInformation(): m_users(makeHandle(new std::vector<CHandlePtr<common::User> >()))
    {
        applyMarkQuery = new ApplyMarkQuery(this);
        availableChannelsListQuery = new AvailableChannelsListQuery(this);
        rssFeedQuery = new RSSFeedQuery(this);
        subscribeChannelQuery = new SubscribeChannelQuery(this);
        subscribedChannelsListQuery = new SubscribedChannelsListQuery(this);
        QTimer *timer = new QTimer(this);
        timer->setInterval(15 * 1000); /* one time per 15 seconds */

        
        connect(applyMarkQuery, SIGNAL(responseReceived(int)),
                this, SLOT(onApplyMarkQueryResponseReceived(int)));
        connect(availableChannelsListQuery, SIGNAL(responseReceived(CHandlePtr<common::Channels>&)),
                this, SLOT(onAvailableChannelsListQueryResponseReceived(CHandlePtr<common::Channels>&)));
        connect(rssFeedQuery, SIGNAL(responseReceived(CHandlePtr<common::DataMarks>&)),
                this, SLOT(onRSSFeedQueryResponseReceived(CHandlePtr<common::DataMarks>&)));
        connect(subscribeChannelQuery, SIGNAL(responseReceived(int)),
                this, SLOT(onSubscribeChannelQueryResponseReceived(int)));
        connect(subscribedChannelsListQuery, SIGNAL(responseReceived(CHandlePtr<common::Channels>&)),
                this, SLOT(onSubscribedChannelsListQueryResponseReceived(CHandlePtr<common::Channels>&)));
        connect(timer, SIGNAL(timeout()), this, SLOT(updateAvailableChannels()));

        updateAvailableChannels();
        timer->start();
    }

    OnLineInformation::~OnLineInformation()
    {

    }

    OnLineInformation& OnLineInformation::getInstance()
    {
        static OnLineInformation onLineInformation;
        return onLineInformation;
    }

    CHandlePtr<common::DataMarks> OnLineInformation::getMarks() const
    {
        return m_marks;
    }

    CHandlePtr<common::Channels> OnLineInformation::getAvailableChannels() const
    {
        return m_availableChannels;
    }

    CHandlePtr<common::Channels> OnLineInformation::getSubscribedChannels() const
    {
        return m_subscribedChannels;
    }

    CHandlePtr<common::User> OnLineInformation::getUser() const
    {
        return m_user;
    }
    CHandlePtr<std::vector<CHandlePtr<common::User> > > OnLineInformation::getUsers() const
    {
        return m_users;
    }

    CHandlePtr<common::Channel> OnLineInformation::findChannel(const std::string& channel) const
    {
        CHandlePtr<common::Channel> empty;

        for (int i = 0; i < m_availableChannels->size(); i++)
        {
            if (m_availableChannels->at(i)->getName() == channel)
                return m_availableChannels->at(i);
        }
        return empty;
    }

    void OnLineInformation::updateMarks()
    {
        rssFeedQuery->setQuery(DEFAULT_USER,
                               common::GpsInfo::getInstance().getLatitude(),
                               common::GpsInfo::getInstance().getLongitude(),
                               DEFAULT_RADIUS);
        rssFeedQuery->doRequest();
    }

    void OnLineInformation::updateAvailableChannels()
    {
        availableChannelsListQuery->setQuery(DEFAULT_USER,
                                             common::GpsInfo::getInstance().getLatitude(),
                                             common::GpsInfo::getInstance().getLongitude(),
                                             DEFAULT_RADIUS);
        availableChannelsListQuery->doRequest();
    }

    void OnLineInformation::updateSubscribedChannels()
    {
        subscribedChannelsListQuery->setQuery(DEFAULT_USER);
        subscribedChannelsListQuery->doRequest();
    }

    void OnLineInformation::subscribeChannel(QString channel)
    {
        subscribeChannelQuery->setQuery(DEFAULT_USER,
                                        channel);
        subscribeChannelQuery->doRequest();
    }

    void OnLineInformation::applyMark(QString channel, QString title, QString link,
                                      QString description)
    {
        applyMarkQuery->setQuery(DEFAULT_USER, channel, title, link, description,
                                 common::GpsInfo::getInstance().getLatitude(),
                                 common::GpsInfo::getInstance().getLongitude(),
                                 QLocale("english").toString(QDateTime::currentDateTime(),"dd MMM yyyy hh:mm:ss"));

        //QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss"));
        applyMarkQuery->doRequest();
    }

    void OnLineInformation::setUser(CHandlePtr<common::User>user)
    {
        m_user = user;
    }

    void OnLineInformation::onApplyMarkQueryResponseReceived(int state)
    {
        updateMarks();
        emit markApplied(state);
    }

    void OnLineInformation::onAvailableChannelsListQueryResponseReceived
            (CHandlePtr<common::Channels> &channels)
    {
        m_availableChannels = channels;
        updateMarks();
        if (m_subscribedChannels == 0)
        {
            updateSubscribedChannels();
        }
        emit availableChannelsUpdated(m_availableChannels);
    }

    void OnLineInformation::onRSSFeedQueryResponseReceived
            (CHandlePtr<common::DataMarks> &marks)
    {
        m_marks = marks;
        emit marksUpdated(m_marks);
    }

    void OnLineInformation::onSubscribeChannelQueryResponseReceived(int state)
    {
        updateSubscribedChannels();
        //updateMarks();
        emit subscribedOnChannel(state);
    }

    void OnLineInformation::onSubscribedChannelsListQueryResponseReceived
            (CHandlePtr<common::Channels> &channels)
    {
        m_subscribedChannels = channels;
        emit subscribedChannelsUpdated(m_subscribedChannels);
    }

} // namespace GUI

/* ===[ End of file $HeadURL$ ]=== */
