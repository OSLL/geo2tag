
#include <qjson/parser.h>
#include <qjson/serializer.h>

#include "GetTimeSlotMarkRequestJSON.h"
#include "DataMarks.h"
#include "JsonChannel.h"
#include "JsonDataMark.h"
#include "JsonUser.h"

GetTimeSlotMarkRequestJSON::GetTimeSlotMarkRequestJSON(QObject *parent) : JsonSerializer(parent)
{
}


QByteArray GetTimeSlotMarkRequestJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap request;

  request.insert("auth_token", m_usersContainer->at(0)->getToken());
  request.insert("mark_id", m_tagsContainer->at(0)->getId());

  return serializer.serialize(request);
}


void GetTimeSlotMarkRequestJSON::parseJson(const QByteArray &data)
{
  clearContainers();
  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();
  if (!ok)
  {
    qFatal("An error occured during parsing json with channel list");
    return;
  }

  QString token = result["auth_token"].toString();
  qlonglong markId = result["mark_id"].toLongLong();

  m_usersContainer->push_back(QSharedPointer<common::User>(new JsonUser("unknown","unknown", token)));
  JsonDataMark* jsonMark = new JsonDataMark(0,0,"unknown", "unknown", "unknown", QDateTime());
  jsonMark->setId(markId);
  m_tagsContainer->push_back(QSharedPointer<DataMark> (jsonMark));
}
