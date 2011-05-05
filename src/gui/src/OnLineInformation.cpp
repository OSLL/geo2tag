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
#include "defines.h"

#include "GpsInfo.h"

namespace GUI
{
  OnLineInformation::OnLineInformation(): m_users(QSharedPointer<Users>())
  {
    m_marks = QSharedPointer<DataMarks>();
    m_availableChannels = QSharedPointer<Channels>();
    m_subscribedChannels = QSharedPointer<Channels>();

    applyMarkQuery = new ApplyMarkQuery(this);
    availableChannelsListQuery = new AvailableChannelsListQuery(this);
    rssFeedQuery = new RSSFeedQuery(this);
    subscribeChannelQuery = new SubscribeChannelQuery(this);
    unsubscribeChannelQuery = new UnsubscribeChannelQuery(this);
    subscribedChannelsListQuery = new SubscribedChannelsListQuery(this);
    loginQuery = new LoginQuery(this);
    m_updateTimer = new QTimer(this);
                                        /* one time per 15 seconds */
    m_updateTimer->setInterval(15 * 1000);

    connect(applyMarkQuery, SIGNAL(responseReceived(QString,QString)),
      this, SLOT(onApplyMarkQueryResponseReceived(QString,QString)));
    connect(availableChannelsListQuery, SIGNAL(responseReceived(QSharedPointer<Channels>&)),
      this, SLOT(onAvailableChannelsListQueryResponseReceived(QSharedPointer<Channels>&)));
    connect(rssFeedQuery, SIGNAL(responseReceived(QSharedPointer<DataMarks>&)),
      this, SLOT(onRSSFeedQueryResponseReceived(QSharedPointer<DataMarks>&)));
    connect(subscribeChannelQuery, SIGNAL(responseReceived(QString,QString)),
      this, SLOT(onSubscribeChannelQueryResponseReceived(QString,QString)));
    connect(unsubscribeChannelQuery, SIGNAL(responseReceived(QString,QString)),
      this, SLOT(onUnsubscribeChannelQueryResponseReceived(QString,QString)));
    connect(subscribedChannelsListQuery, SIGNAL(responseReceived(QSharedPointer<Channels>&)),
      this, SLOT(onSubscribedChannelsListQueryResponseReceived(QSharedPointer<Channels>&)));
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateAvailableChannels()));
    connect(loginQuery, SIGNAL(responseReceived(QString,QString,QString)),
      this, SLOT(onLoginQueryResponseReceived(QString,QString,QString)));

  }

  OnLineInformation::~OnLineInformation()
  {

  }

  OnLineInformation& OnLineInformation::getInstance()
  {
    static OnLineInformation onLineInformation;
    return onLineInformation;
  }

  QSharedPointer<DataMarks> OnLineInformation::getMarks() const
  {
    return m_marks;
  }

  QSharedPointer<Channels> OnLineInformation::getAvailableChannels() const
  {
    return m_availableChannels;
  }

  QSharedPointer<Channels> OnLineInformation::getSubscribedChannels() const
  {
    return m_subscribedChannels;
  }

  QSharedPointer<User> OnLineInformation::getUser() const
  {
    return m_user;
  }
  QSharedPointer<Users> OnLineInformation::getUsers() const
  {
    return m_users;
  }

  QSharedPointer<Channel> OnLineInformation::findChannel(const QString& channel) const
  {
    QSharedPointer<Channel> empty;

    for (int i = 0; i < m_availableChannels->size(); i++)
    {
      if (m_availableChannels->at(i)->getName() == channel)
        return m_availableChannels->at(i);
    }
    return empty;
  }

  void OnLineInformation::updateMarks()
  {
    if (m_auth_token != QString(""))
    {
      rssFeedQuery->setQuery(m_user,
        common::GpsInfo::getInstance().getLatitude(),
        common::GpsInfo::getInstance().getLongitude(),
        double(DEFAULT_RADIUS));
      rssFeedQuery->doRequest();
    }
  }

  void OnLineInformation::updateAvailableChannels()
  {
    if (m_auth_token != QString(""))
    {
      availableChannelsListQuery->setQuery(m_auth_token,
        common::GpsInfo::getInstance().getLatitude(),
        common::GpsInfo::getInstance().getLongitude(),
        double(DEFAULT_RADIUS));
      availableChannelsListQuery->doRequest();
    }
  }

  void OnLineInformation::updateSubscribedChannels()
  {
    if (m_auth_token != QString(""))
    {
      subscribedChannelsListQuery->setQuery(m_auth_token);
      subscribedChannelsListQuery->doRequest();
    }
  }

  void OnLineInformation::subscribeChannel(QString channel)
  {
    if (m_auth_token != QString(""))
    {
      subscribeChannelQuery->setQuery(m_auth_token,
        channel);
      subscribeChannelQuery->doRequest();
    }
  }

  void OnLineInformation::unsubscribeChannel(QString channel)
  {
    if (m_auth_token != QString(""))
    {
      unsubscribeChannelQuery->setQuery(m_auth_token,
        channel);
      unsubscribeChannelQuery->doRequest();
    }
  }

  void OnLineInformation::applyMark(QString channel, QString title, QString link,
    QString description)
  {
    if (m_auth_token != QString(""))
    {
      applyMarkQuery->setQuery(m_auth_token, channel, title, link, description,
        common::GpsInfo::getInstance().getLatitude(),
        common::GpsInfo::getInstance().getLongitude(),
        QLocale("english").toString(QDateTime::currentDateTime(),"dd MMM yyyy hh:mm:ss"));

      //QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss"));
      applyMarkQuery->doRequest();
    }
  }

  void OnLineInformation::setAuthToken(QString token)
  {
    m_auth_token = token;
    if (token != QString(""))
    {
      m_updateTimer->start();
    }
    else
    {
      m_updateTimer->stop();
    }
  }

  void OnLineInformation::onApplyMarkQueryResponseReceived(QString status,QString status_description)
  {
    if (status == QString("Ok"))
    {
      updateMarks();
      emit markApplied(1);
    }
    else
    {
      emit markApplied(0);
    }
  }

  void OnLineInformation::onAvailableChannelsListQueryResponseReceived
    (QSharedPointer<Channels> &channels)
  {
    m_availableChannels->clear();
    for (size_t i = 0; i < channels->size(); i++)
    {
      QSharedPointer<Channel> channel = (*channels)[i];
      m_availableChannels->push_back(channel);
      channel->setDisplayed(true);
    }
    updateMarks();
    if (m_subscribedChannels->size() == 0)
    {
      updateSubscribedChannels();
    }
    emit availableChannelsUpdated(m_availableChannels);
  }

  void OnLineInformation::onRSSFeedQueryResponseReceived
    (QSharedPointer<DataMarks> &marks)
  {
    m_marks->clear();
    for(size_t i=0; i<marks->size(); ++i)
    {
      QSharedPointer<DataMark> mark = (*marks)[i];
      m_marks->push_back(mark);
    }

    emit marksUpdated(m_marks);
  }

  void OnLineInformation::onSubscribeChannelQueryResponseReceived(QString status,QString status_description)
  {
    updateSubscribedChannels();
    updateMarks();
    if (status == QString("Ok"))
    {
      emit subscribedChannel();
    }
  }

  void OnLineInformation::onUnsubscribeChannelQueryResponseReceived(QString status,QString status_description)
  {
    updateSubscribedChannels();
    updateMarks();
    if (status == QString("Ok"))
    {
      emit unsubscribedChannel();
    }
  }

  void OnLineInformation::onSubscribedChannelsListQueryResponseReceived
    (QSharedPointer<Channels> &channels)
  {
    m_subscribedChannels->clear();
    for (size_t i = 0; i < channels->size(); i++)
    {
      QSharedPointer<Channel> channel = (*channels)[i];
      m_subscribedChannels->push_back(channel);
    }
    emit subscribedChannelsUpdated(m_subscribedChannels);
  }

  void OnLineInformation::onLoginQueryResponseReceived(QString status, QString auth_token,QString status_description)
  {
    if (status == QString("Ok"))
    {
      m_auth_token = auth_token;
      updateAvailableChannels();
    }
  }
}                                       // namespace GUI


/* ===[ End of file $HeadURL$ ]=== */
