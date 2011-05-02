#ifndef CHANNELSWIDGET_H
#define CHANNELSWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WSelectionBox>
#include <Wt/WPushButton>
#include <Wt/WTableView>
#include "Channel.h"
#include "ChannelsModel.h"
#include <QSharedPointer>
#include "User.h"
using namespace Wt;

class ChannelsWidget : public WContainerWidget
{
  QSharedPointer<User> m_user;
  //    std::string m_token;
  WText *channelsText;
  WTableView *channelsTable;
  ChannelsModel *channelsModel;

  //    QSharedPointer<Channels> m_subscribedChannels;
  //    QSharedPointer<Channels> m_availableChannels;

  public:
    ChannelsWidget(const QSharedPointer<User> user, WContainerWidget *parent = 0);
    void updateChannelsModel();

    /* signals */
    Wt::Signal<> channelsUpdated;

    /* slots */
    void onModelUpdated();

};
/* CHANNELSWIDGET_H */
#endif
