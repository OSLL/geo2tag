#include "ApplyChannelQuery.h"
#include "defines.h"
#include "AddChannelRequestJSON.h"
#include "AddChannelResponseJSON.h"

ApplyChannelQuery::ApplyChannelQuery(const QSharedPointer<Channel> &channel, const QSharedPointer<Session> &session, QObject *parent)
    : DefaultQuery(parent),
      m_channel(channel),
      m_session(session)
{
}


ApplyChannelQuery::ApplyChannelQuery(QObject *parent): DefaultQuery(parent)
{
}


void ApplyChannelQuery::setQuery(const QSharedPointer<Channel> &channel, const QSharedPointer<Session> &session)
{
  m_channel = channel;
  m_session = session;
}


QString ApplyChannelQuery::getUrl() const
{
  return APPLY_CHANNEL_HTTP_URL;
}


QByteArray ApplyChannelQuery::getRequestBody() const
{
  AddChannelRequestJSON request;
  request.addChannel(m_channel);
  request.addSession(m_session);
  return request.getJson();
}


/*void ApplyChannelQuery::processReply(QNetworkReply *reply)
{
  AddChannelResponseJSON response;
  response.parseJson(reply->readAll());
  m_errno = response.getErrno();
  if(response.getErrno() == SUCCESS)
  {
    Q_EMIT channelAdded(m_channel);
  }
  else
  {
    Q_EMIT errorOccured(response.getErrno());
  }
}*/


ApplyChannelQuery::~ApplyChannelQuery()
{

}


const QSharedPointer<Channel>& ApplyChannelQuery::getChannel() const
{
  return m_channel;
}
