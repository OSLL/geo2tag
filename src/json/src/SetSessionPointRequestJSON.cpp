#include <qjson/parser.h>
#include <qjson/serializer.h>

#include "SetSessionPointRequestJSON.h"
#include "JsonUser.h"

SetSessionPointRequestJSON::SetSessionPointRequestJSON(QObject *parent) : JsonSerializer(parent)
{
}


void SetSessionPointRequestJSON::parseJson(const QByteArray &data)
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

  QString authToken = result["auth_token"].toString();
  m_usersContainer->push_back(QSharedPointer<User>(new JsonUser("unknown","unknown", authToken)));

  m_latitude = result["latitude"].toDouble();
  m_longitude = result["longitude"].toDouble();
  m_radius = result["radius"].toDouble();
  m_timeSlot = result["timeSlot"].toULongLong();
  m_isCurrentTime = result["isCurrentTime"].toBool();
  if (!m_isCurrentTime)
    m_time = QDateTime::fromString(result["time"].toString(), "dd MM yyyy HH:mm:ss.zzz");
}


QByteArray SetSessionPointRequestJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap obj;

  obj.insert("auth_token", m_usersContainer->at(0)->getToken());
  obj.insert("latitude", getLatitude());
  obj.insert("longitude", getLongitude());
  obj.insert("radius", getRadius());
  obj.insert("timeSlot", getTimeSlot());
  obj.insert("isCurrentTime", getIsTimeCurrent());
  obj.insert("time", getTime());

  return serializer.serialize(obj);
}


double SetSessionPointRequestJSON::getLatitude() const
{
  return m_latitude;
}


double SetSessionPointRequestJSON::getLongitude() const
{
  return m_longitude;
}


double SetSessionPointRequestJSON::getRadius() const
{
  return m_radius;
}


qulonglong SetSessionPointRequestJSON::getTimeSlot() const
{
  return m_timeSlot;
}


const QDateTime& SetSessionPointRequestJSON::getTime() const
{
  return m_time;
}


bool SetSessionPointRequestJSON::getIsTimeCurrent() const
{
  return m_isCurrentTime;
}


void SetSessionPointRequestJSON::setLatitude(double latitude)
{
  m_latitude = latitude;
}


void SetSessionPointRequestJSON::setLongitude(double longitude)
{
  m_longitude = longitude;
}


void SetSessionPointRequestJSON::setRadius(double radius)
{
  m_radius = radius;
}


void SetSessionPointRequestJSON::setTimeSlot(const qulonglong& timeSlot)
{
  m_timeSlot = timeSlot;
}


void SetSessionPointRequestJSON::setTime(const QDateTime& time)
{
  m_time = time;
}


void SetSessionPointRequestJSON::setIsTimeCurrent(bool fl)
{
  m_isCurrentTime = fl;
}
