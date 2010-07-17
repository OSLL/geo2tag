#ifndef CHANNELSWIDGET_H
#define CHANNELSWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WSelectionBox>
#include <Wt/WPushButton>

#include "Channel.h"

using namespace Wt;

class ChannelsWidget : public WContainerWidget
{
    WText *channelsText;
    WSelectionBox *channelsBox;
    WPushButton *subscribeButton;
    WPushButton *unsubscribeButton;

    CHandlePtr<common::Channels> m_subscribedChannels;
    CHandlePtr<common::Channels> m_availableChannels;

public:
    ChannelsWidget(WContainerWidget *parent = 0);
    void updateChannelsBox();

    /* slots */
    void onSubscribeClicked();
    void onUnsubscribeClicked();

};


#endif /* CHANNELSWIDGET_H */
