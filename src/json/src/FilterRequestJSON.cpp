/*
 * Copyright 2011  bac1ca  bac1ca89@gmail.com
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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
 * \file FilterRequestJSON.cpp
 * \brief FilterRequestJSON implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "FilterRequestJSON.h"
#include "JsonChannel.h"

#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
#include <qjson/parser.h>
#include <qjson/serializer.h>
#else
#include "parser.h"
#include "serializer.h"
#endif

FilterRequestJSON::FilterRequestJSON(QObject *parent) : JsonSerializer(parent)
{
}


FilterRequestJSON::~FilterRequestJSON()
{
}


void FilterRequestJSON::setShape(const QSharedPointer<FShape> &shape)
{
  m_shape = shape;
}


QSharedPointer<FShape>& FilterRequestJSON::getShape()
{
  return m_shape;
}


void FilterRequestJSON::setTimeFrom(QDateTime timeFrom)
{
  m_timeFrom = timeFrom;
}


QDateTime FilterRequestJSON::getTimeFrom() const
{
  return m_timeFrom;
}


void FilterRequestJSON::setTimeTo(QDateTime timeTo)
{
  m_timeTo = timeTo;
}


QDateTime FilterRequestJSON::getTimeTo() const
{
  return m_timeTo;
}


void FilterRequestJSON::setAltitude1(double alt)
{
  m_alt1 = alt;
}


void FilterRequestJSON::setAltitude2(double alt)
{
  m_alt2 = alt;
}


double FilterRequestJSON::getAltitude1() const
{
  return m_alt1;
}


void FilterRequestJSON::setChannel(const QSharedPointer<Channel> &channel)
{
  m_channelsContainer->push_back(channel);
}


double FilterRequestJSON::getAltitude2() const
{
  return m_alt2;
}


bool FilterRequestJSON::parseJson(const QByteArray& data)
{
  clearContainers();

  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();
  if (!ok) return false;

  QString name = result["channel"].toString();
  if (!name.isEmpty())
  {
    Channel * ch = new JsonChannel(name,"n/a","n/a");
    m_channelsContainer->push_back(QSharedPointer<Channel>(ch));
  }
  return true;
}
