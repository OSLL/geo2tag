/*
 * Copyright 2010-2012 OSLL osll@osll.spb.ru
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
 * \file  BasicUser.cpp
 * \brief Implementation of BasicUser class
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "BasicUser.h"

namespace common
{

  BasicUser::BasicUser(const QString &name, const QString &passw, const QString &email)
    : m_login(name),
    m_password(passw),
    m_email(email)
  {
  }

  qlonglong BasicUser::getId() const
  {
    return 0;
  }

  const QString& BasicUser::getPassword() const
  {
    return m_password;
  }

  const QString& BasicUser::getLogin() const
  {
    return m_login;
  }

  const QString& BasicUser::getEmail() const
  {
    return m_email;
  }

  void BasicUser::setPassword(QString password)
  {
    m_password = password;
  }

  BasicUser::~BasicUser()
  {
  }

}                                       //namespace common
