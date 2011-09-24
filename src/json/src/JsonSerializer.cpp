#include "JsonSerializer.h"

JsonSerializer::JsonSerializer(QObject * parent):QObject(parent),
m_channelsContainer(new Channels),
m_tagsContainer(new DataMarks),
m_usersContainer(new common::Users)
{
}


JsonSerializer::~JsonSerializer()
{

}


QSharedPointer<common::Users> JsonSerializer::getUsers() const
{
  return m_usersContainer;
}


QSharedPointer<Channels> JsonSerializer::getChannels() const
{
  return m_channelsContainer;
}


QSharedPointer<DataMarks> JsonSerializer::getTags() const
{
  return m_tagsContainer;
}


void JsonSerializer::addChannel(const QSharedPointer<Channel> &channel)
{
  m_channelsContainer->push_back(channel);
}


void JsonSerializer::addTag(const QSharedPointer<DataMark> &tag)
{
  m_tagsContainer->push_back(tag);
}


void JsonSerializer::addUser(const QSharedPointer<common::User> &tag)
{
  m_usersContainer->push_back(tag);
}


void JsonSerializer::clearContainers()
{
  m_usersContainer->clear();
  m_tagsContainer->clear();
  m_channelsContainer->clear();
}


const QString& JsonSerializer::getStatus() const
{
  return m_status;
}


const QString& JsonSerializer::getStatusMessage() const
{
  return m_statusMessage;
}


void JsonSerializer::setStatus(const QString &s)
{
  m_status = s;
}


void JsonSerializer::setStatusMessage(const QString &s)
{
  m_statusMessage = s;
}
