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
 *
 *
 * \file Channel.cpp
 * \brief Channel implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */
#include <QDebug>
#include "Channel.h"

const qulonglong Channel::DEFAULT_TIME_SLOT_VALUE_MIN = 525600;

Channel::Channel(const QString &name,
const QString &description,
const QString &url,
const QSharedPointer<common::BasicUser>& owner):
m_name(name),
m_description(description),
m_url(url),
m_owner(owner),
m_isDisplayed(true)
{
  m_activeRadius = 5.0;                 // 5 km
}


qlonglong Channel::getId() const
{
  // Database doesn't contain 0 in sequences, see scripts/base.sql
  return 0;
}


const QString& Channel::getDescription() const
{
  return m_description;
}


const QString& Channel::getName() const
{
  return m_name;
}


void Channel::setDescription(const QString& description)
{
  m_description = description;
}


const QString& Channel::getUrl() const
{
  return m_url;
}


const QSharedPointer<common::BasicUser>& Channel::getOwner() const
{
  return m_owner;
}


void Channel::setUrl(const QString& url)
{
  m_url = url;
}


void Channel::setOwner(const QSharedPointer<common::BasicUser> &owner)
{
  m_owner = owner;
}


bool Channel::isDisplayed() const
{
  return m_isDisplayed;
}


void Channel::setDisplayed(bool fl)
{
  m_isDisplayed = fl;
}


void Channel::setRadius(const double& radius)
{
  m_activeRadius = radius;
}


double Channel::getRadius() const
{
  return m_activeRadius;
}


Channel::~Channel()
{
}


/* ===[ End of file ]=== */
