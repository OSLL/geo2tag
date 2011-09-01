#ifndef CHANNELACTION_H
#define CHANNELACTION_H

#include "User.h"

class ChannelActions: public QObject
{
  Q_OBJECT
    int m_user;
  int m_channel;
  int m_action;

  public:
    ChannelActions(int user, int channel, int action);

    virtual ~ChannelActions();

};
#endif                                  // CHANNELACTION_H
