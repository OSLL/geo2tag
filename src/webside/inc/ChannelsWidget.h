#ifndef CHANNELSWIDGET_H
#define CHANNELSWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WSelectionBox>
#include <Wt/WPushButton>
#include <WTableView>
#include "Channel.h"
#include "ChannelsModel.h"
using namespace Wt;

class ChannelsWidget : public WContainerWidget
{
    std::string m_token;
    WText *channelsText;
    WTableView *channelsTable;
    ChannelsModel *channelsModel;

//    CHandlePtr<common::Channels> m_subscribedChannels;
//    CHandlePtr<common::Channels> m_availableChannels;

public:
    ChannelsWidget(const std::string &token, WContainerWidget *parent = 0);
    void updateChannelsModel();

    /* signals */
    Wt::Signal<> channelsUpdated;

    /* slots */
    void onModelUpdated();

};


#endif /* CHANNELSWIDGET_H */
