/*
 * Copyright 2012 bac1ca  bac1ca89@gmail.com
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
 * \file ErrnoInfoResponseJSON.cpp
 * \brief ErrnoInfoResponseJSON implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef Q_WS_SYMBIAN
#include <qjson/parser.h>
#include <qjson/serializer.h>
#else
#include "parser.h"
#include "serializer.h"
#endif
#include "ErrnoInfoResponseJSON.h"
#include "ErrnoTypes.h"

#include <syslog.h>
#include <QList>

ErrnoInfoResponseJSON::ErrnoInfoResponseJSON(QObject *parent): JsonSerializer(parent)
{
}


QByteArray ErrnoInfoResponseJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap obj;
  QVariantList errnoInfo;

  QMap<int, QString> emap = Errno::initErrnoMap();
  QList<int> keys = emap.keys();

  for (int i = 0; i < keys.size(); i++)
  {
    int key = keys.at(i);
    QVariantMap infoItem;
    infoItem.insert("errno", key);
    infoItem.insert("value", emap.value(key));
    errnoInfo.append(infoItem);
  }

  obj.insert("errno_info", errnoInfo);
  obj.insert("errno", getErrno());
  return serializer.serialize(obj);
}


bool ErrnoInfoResponseJSON::parseJson(const QByteArray&)
{
  //not used in this case
  return false;
}
