

#include "AddChannelRequestJSON.h"
#include "JsonUser.h"
#include "JsonChannel.h"

#ifndef Q_OS_SYMBIAN
#include <qjson/parser.h>
#include <qjson/serializer.h>
#else
#include "parser.h"
#include "serializer.h"
#endif


AddChannelRequestJSON::AddChannelRequestJSON(QObject *parent) : JsonSerializer(parent)
{
}


QByteArray AddChannelRequestJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap obj;
  obj.insert("auth_token", m_usersContainer->at(0)->getToken());
  obj.insert("name", m_channelsContainer->at(0)->getName());
  obj.insert("description",m_channelsContainer->at(0)->getDescription());
  obj.insert("url",m_channelsContainer->at(0)->getUrl());
  return serializer.serialize(obj);
}


void AddChannelRequestJSON::parseJson(const QByteArray&data)
{
  clearContainers();

  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();
  if (!ok)
  {
    qFatal("An error occured during parsing json with channel list");
  }
  QString auth_token = result["auth_token"].toString();
  QString name = result["name"].toString();
  QString description = result["description"].toString();
  QString url = result["url"].toString();
  m_usersContainer->push_back(QSharedPointer<common::User>(new JsonUser("unknown","unknown",auth_token)));
  m_channelsContainer->push_back(QSharedPointer<Channel>(new JsonChannel(name,description,url)));
}
