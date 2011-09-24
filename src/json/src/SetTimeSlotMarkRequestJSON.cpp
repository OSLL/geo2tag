#include <qjson/parser.h>
#include <qjson/serializer.h>

#include "SetTimeSlotMarkRequestJSON.h"
#include "DataMarks.h"
#include "JsonDataMark.h"
#include "JsonUser.h"
#include "JsonTimeSlot.h"

SetTimeSlotMarkRequestJSON::SetTimeSlotMarkRequestJSON(QObject *parent) : JsonSerializer(parent)
{
}


QByteArray SetTimeSlotMarkRequestJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap request;

  request.insert("auth_token", m_usersContainer->at(0)->getToken());
  request.insert("mark_id", m_tagsContainer->at(0)->getId());
  request.insert("timeSlot", m_tagsContainer->at(0)->getTimeSlot()->getSlot());

  return serializer.serialize(request);
}


void SetTimeSlotMarkRequestJSON::parseJson(const QByteArray &data)
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
  qulonglong timeSlot = result["timeSlot"].toULongLong();

  m_usersContainer->push_back(QSharedPointer<common::User>(new JsonUser("unknown","unknown", token)));

  JsonDataMark* jsonMark = new JsonDataMark(0,0,"unknown", "unknown", "unknown", QDateTime());
  jsonMark->setId(markId);
  m_tagsContainer->push_back(QSharedPointer<DataMark> (jsonMark));

  m_tagsContainer->at(0)->setTimeSlot(QSharedPointer<TimeSlot>(new JsonTimeSlot(timeSlot)));
}
