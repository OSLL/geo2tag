#include "AddChannelQuery.h"
#include "defines.h"
#include "AddChannelRequestJSON.h"
#include "AddChannelResponseJSON.h"

AddChannelQuery::AddChannelQuery(const QSharedPointer<common::User> &user,
const QSharedPointer<Channel> &channel,
QObject *parent) :
DefaultQuery(parent), m_user(user), m_channel(channel)
{
}


AddChannelQuery::AddChannelQuery(QObject *parent): DefaultQuery(parent)
{
}


void AddChannelQuery::setQuery(const QSharedPointer<common::User> &user,
const QSharedPointer<Channel> &channel)
{
  m_user = user;
  m_channel = channel;
}


QString AddChannelQuery::getUrl() const
{
  return ADD_CHANNEL_HTTP_URL;
}


void AddChannelQuery::setUrl(const QString &url)
{
  DefaultQuery::setUrl(url + ADD_CHANNEL_HTTP_URL);
}


QByteArray AddChannelQuery::getRequestBody() const
{
  AddChannelRequestJSON request;
  request.addChannel(m_channel);
  request.addUser(m_user);
  return request.getJson();
}


void AddChannelQuery::processReply(QNetworkReply *reply)
{
  AddChannelResponseJSON response;
  response.parseJson(reply->readAll());
  if(response.getErrno() == SUCCESS)
  {
    Q_EMIT channelAdded();
  }
  else
  {
    Q_EMIT errorOccured(getErrnoDescription(response.getErrno()));
  }
}


AddChannelQuery::~AddChannelQuery()
{
}


QSharedPointer<common::User> AddChannelQuery::getUser() const
{
  return m_user;
}


QSharedPointer<Channel> AddChannelQuery::getChannel() const
{
  return m_channel;
}
