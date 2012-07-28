/*
 * Copyright 2010-2012  OSLL osll@osll.spb.ru
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
/*----------------------------------------------------------------- !
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include <QDebug>

#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
#include <qjson/parser.h>
#include <qjson/serializer.h>
#else
#include "parser.h"
#include "serializer.h"
#endif

#include "RegisterUserResponseJSON.h"
#include "JsonUser.h"
#include "ErrnoTypes.h"

RegisterUserResponseJSON::RegisterUserResponseJSON(QObject *parent)
: JsonSerializer(parent)
{
}


void RegisterUserResponseJSON::setConfirmUrl(const QString& url)
{
  m_confirmUrl = url;
}


const QString& RegisterUserResponseJSON::getConfirmUrl() const
{
  return m_confirmUrl;
}


QByteArray RegisterUserResponseJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap obj;
  obj.insert("errno", m_errno);
  if (m_errno == SUCCESS) obj.insert("confirmUrl", m_confirmUrl);
  return serializer.serialize(obj);
}


bool RegisterUserResponseJSON::parseJson(const QByteArray &data)
{
  //clearContainers();
  QJson::Parser parser;
  bool ok;

  QVariantMap result = parser.parse(data, &ok).toMap();
  if (!ok) return false;

  result["errno"].toInt(&ok);
  if (!ok) return false;
  m_errno = result["errno"].toInt(&ok);
  m_confirmUrl = result["confirmUrl"].toString();

  return true;
}
