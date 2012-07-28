#include "SubscribeChannelQuery.h"
#include "defines.h"
#include "SubscribeChannelJSON.h"
#include "SubscribeChannelResponseJSON.h"

SubscribeChannelQuery::SubscribeChannelQuery(const QSharedPointer<Channel> &channel,
const QSharedPointer<Session> &session, QObject *parent)
: DefaultQuery(parent),
m_channel(channel),
m_session(session)
{

}


SubscribeChannelQuery::SubscribeChannelQuery(QObject *parent): DefaultQuery(parent)
{
}


void SubscribeChannelQuery::setQuery(const QSharedPointer<Channel> &channel, const QSharedPointer<Session> &session)
{
  m_channel = channel;
  m_session = session;
}


QString SubscribeChannelQuery::getUrl() const
{
  return SUBSCRIBE_HTTP_URL;
}


QByteArray SubscribeChannelQuery::getRequestBody() const
{
  SubscribeChannelRequestJSON request;
  request.addChannel(m_channel);
  request.addSession(m_session);
  return request.getJson();
}


SubscribeChannelQuery::~SubscribeChannelQuery()
{

}
