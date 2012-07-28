#ifndef APPLYCHANNELQUERY_H
#define APPLYCHANNELQUERY_H
#include <QObject>
#include <QString>
#include "DefaultQuery.h"
#include "Session.h"
#include "Channel.h"

class ApplyChannelQuery: public DefaultQuery
{
  Q_OBJECT

    QSharedPointer<Channel> m_channel;
  QSharedPointer<Session> m_session;

  virtual QString getUrl() const;
  virtual QByteArray getRequestBody() const;

  public:

    ApplyChannelQuery(const QSharedPointer<Channel> &channel, const QSharedPointer<Session> &session, QObject *parent = 0);

    ApplyChannelQuery(QObject *parent = 0);

    void setQuery(const QSharedPointer<Channel> &channel, const QSharedPointer<Session> &session);

    const QSharedPointer<Channel>& getChannel() const;

    ~ApplyChannelQuery();

};
#endif                                  // APPLYCHANNELQUERY_H
