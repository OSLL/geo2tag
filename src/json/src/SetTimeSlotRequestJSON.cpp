#include <qjson/parser.h>
#include <qjson/serializer.h>

#include "SetTimeSlotRequestJSON.h"
#include "DataMarks.h"
#include "JsonChannel.h"
#include "JsonDataMark.h"
#include "JsonUser.h"
#include "JsonTimeSlot.h"

SetTimeSlotRequestJSON::SetTimeSlotRequestJSON(QObject *parent) : JsonSerializer(parent)
{
}


QByteArray SetTimeSlotRequestJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap request;

  request.insert("auth_token", m_usersContainer->at(0)->getToken());
  request.insert("channel", m_channelsContainer->at(0)->getName());
  request.insert("timeSlot", m_channelsContainer->at(0)->getTimeSlot()->getSlot());

  return serializer.serialize(request);
}


void SetTimeSlotRequestJSON::parseJson(const QByteArray &data)
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
  QString channel = result["channel"].toString();
  qulonglong timeSlot = result["timeSlot"].toULongLong();

  m_usersContainer->push_back(QSharedPointer<common::User>(new JsonUser("unknown","unknown", token)));
  m_channelsContainer->push_back(QSharedPointer<Channel> (new JsonChannel(channel, "unknown", "unknown")));
  m_channelsContainer->at(0)->setTimeSlot(QSharedPointer<TimeSlot>(new JsonTimeSlot(timeSlot)));
}
