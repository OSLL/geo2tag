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
/*!
 * \file User.h
 * \brief Header of User
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _User_H_83C39FC3_ECFB_41CD_8902_8176172CD890_INCLUDED_
#define _User_H_83C39FC3_ECFB_41CD_8902_8176172CD890_INCLUDED_

#include <QString>
#include <QSharedPointer>
#include <typeinfo>
#include "Channel.h"
#include "BasicUser.h"

//#include "ConcurrentVector.h"

namespace common
{

  class User: public QObject, public BasicUser
  {
    Q_OBJECT

      QString m_result;
    QString m_token;
    // list of subscribed channels
    QSharedPointer<Channels> m_channels;

    public:

      User(const QString & name="", const QString & passw="", const QString & email="");

      virtual qlonglong getId() const;

      void subscribe(const QSharedPointer<Channel>& channel);

      void unsubscribe(const QSharedPointer<Channel>& channel);

      const QSharedPointer<Channels> getSubscribedChannels() const;

      virtual ~User();
      // class User
  };

  typedef ConcurrentVector<User> Users;

}                                       //namespace common


//_User_H_83C39FC3_ECFB_41CD_8902_81D6172CD890_INCLUDED_
#endif

/* ===[ End of file ]=== */
