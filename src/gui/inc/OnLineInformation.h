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

/* $Id$ */
/*!
 * \file OnLineInformation.h
 * \brief Header of OnLineInformation
 * \todo add comment here
 *
 * File description
 *
 * PROJ: geo2tag
 * ---------------------------------------------------------------- */

#ifndef _OnLineInformation_H_91CA39C8_1612_4CBC_B6C2_8ED74E337584_INCLUDED_
#define _OnLineInformation_H_91CA39C8_1612_4CBC_B6C2_8ED74E337584_INCLUDED_

#include <QObject>
#include <QTimer>

#include "DataMarks.h"
#include "Channel.h"
#include "User.h"
#include "ApplyMarkQuery.h"
#include "AvailableChannelsListQuery.h"
#include "RSSFeedQuery.h"
#include "SubscribeChannelQuery.h"
#include "UnsubscribeChannelQuery.h"
#include "SubscribedChannelsListQuery.h"
#include "LoginQuery.h"

namespace GUI
{
  /*!
   * Object of this class contains up to date information about
   * marks and available and subscribed channels.
   * Users of this class can request to update the information
   * and get new information through signals.
   *
   *
   */
  class OnLineInformation : public QObject
  {
    Q_OBJECT

      QSharedPointer<DataMarks> m_marks;
    QSharedPointer<Channels>  m_availableChannels;
    QSharedPointer<Channels>  m_subscribedChannels;
    QSharedPointer<Users > m_users;

    /* We need something that represents user */
    QSharedPointer<User> m_user;
    QString m_userName;
    QString m_userPassword;
    QString m_auth_token;

    QTimer                      * m_updateTimer;
    ApplyMarkQuery              * applyMarkQuery;
    AvailableChannelsListQuery  * availableChannelsListQuery;
    RSSFeedQuery                * rssFeedQuery;
    SubscribeChannelQuery       * subscribeChannelQuery;
    UnsubscribeChannelQuery     * unsubscribeChannelQuery;
    SubscribedChannelsListQuery * subscribedChannelsListQuery;
    LoginQuery                  * loginQuery;

    OnLineInformation();
    public:

      ~OnLineInformation();
      static OnLineInformation& getInstance();

      QSharedPointer<DataMarks> getMarks() const;
      QSharedPointer<Channels> getAvailableChannels() const;
      QSharedPointer<Channels> getSubscribedChannels() const;
      QSharedPointer<User> getUser() const;
      QSharedPointer<Users> getUsers() const;
      QSharedPointer<Channel> findChannel(const QString& channel) const;

    public slots:
      void updateMarks();
      void updateAvailableChannels();
      void updateSubscribedChannels();
      void subscribeChannel(QString channel);
      void unsubscribeChannel(QString channel);
      void applyMark(QString channel, QString title, QString link,
        QString description);
      void setAuthToken(QString token);

      signals:
      void marksUpdated(QSharedPointer<DataMarks> );
      void availableChannelsUpdated(QSharedPointer<Channels> );
      void subscribedChannelsUpdated(QSharedPointer<Channels> );
      void subscribedChannel();
      void unsubscribedChannel();
      void markApplied(int status);

    private slots:
      void onApplyMarkQueryResponseReceived(QString status,QString status_description);
      void onAvailableChannelsListQueryResponseReceived(QSharedPointer<Channels>& channels);
      void onRSSFeedQueryResponseReceived(QSharedPointer<DataMarks>& marks);
      void onSubscribeChannelQueryResponseReceived(QString status,QString status_description);
      void onUnsubscribeChannelQueryResponseReceived(QString status,QString status_description);
      void onSubscribedChannelsListQueryResponseReceived(QSharedPointer<Channels>& channels);
      void onLoginQueryResponseReceived(QString status, QString auth_token,QString status_description);

    private:
      OnLineInformation(const OnLineInformation& obj);
      OnLineInformation& operator=(const OnLineInformation& obj);

      // class OnLineInformation
  };

  // namespace GUI
}


//_OnLineInformation_H_91CA39C8_1612_4CBC_B6C2_8ED74E337584_INCLUDED_
#endif

/* ===[ End of file $HeadURL$ ]=== */
