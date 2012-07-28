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
 * \file  Session.cpp
 * \brief Implementation of the class Session
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "common/inc/Session.h"

Session::Session(const QString &sessionToken, const QDateTime &lastAccessTime, const QSharedPointer<common::User> &user)
: m_sessionToken(sessionToken),
m_lastAccessTime(lastAccessTime),
m_user(user)
{
}


void Session::setSessionToken(const QString &sessionToken)
{
  m_sessionToken = sessionToken;
}


void Session::setLastAccessTime(const QDateTime lastAccessTime)
{
  m_lastAccessTime = lastAccessTime;
}


void Session::setUser(const QSharedPointer<common::User> &user)
{
  m_user = user;
}


const QString& Session::getSessionToken() const
{
  return m_sessionToken;
}


const QDateTime& Session::getLastAccessTime() const
{
  return m_lastAccessTime;
}


const QSharedPointer<common::User>& Session::getUser() const
{
  return m_user;
}


qlonglong Session::getId() const
{
  return 0;
}


Session::~Session()
{
}
