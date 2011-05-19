
#include <qjson/parser.h>
#include <qjson/serializer.h>

#include "GetSessionPointResponseJSON.h"
#include "JsonUser.h"

GetSessionPointResponseJSON::GetSessionPointResponseJSON(QObject *parent) : JsonSerializer(parent)
{
}


void GetSessionPointResponseJSON::parseJson(const QByteArray &data)
{
  clearContainers();

  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();

  if (!ok)
  {
    qFatal("An error occured during parsing json with rss request");
    return;
  }

  m_latitude = result["latitude"].toDouble();
  m_longitude = result["longitude"].toDouble();
  m_radius = result["radius"].toDouble();
  m_timeSlot = result["timeSlot"].toULongLong();
  m_time = QDateTime::fromString(result["time"].toString(), "dd MM yyyy HH:mm:ss.zzz");
}


QByteArray GetSessionPointResponseJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap obj;

  QSharedPointer<User> user = m_usersContainer->at(0);

  obj.insert("latitude", user->getSession()->getLatitude());
  obj.insert("longitude", user->getSession()->getLongitude());
  obj.insert("radius", user->getSession()->getRadius());
  obj.insert("timeSlot", user->getSession()->getTimeSlot());
  obj.insert("time", user->getSession()->getTime());

  return serializer.serialize(obj);
}


double GetSessionPointResponseJSON::getLatitude() const
{
  return m_latitude;
}


double GetSessionPointResponseJSON::getLongitude() const
{
  return m_longitude;
}


double GetSessionPointResponseJSON::getRadius() const
{
  return m_radius;
}


qulonglong GetSessionPointResponseJSON::getTimeSlot() const
{
  return m_timeSlot;
}


const QDateTime& GetSessionPointResponseJSON::getTime() const
{
  return m_time;
}
