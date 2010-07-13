#ifndef CHANNELSWIDGET_H
#define CHANNELSWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WSelectionBox>
#include <Wt/WPushButton>

using namespace Wt;

class ChannelsWidget : public WContainerWidget
{
    WText *channelsText;
    WSelectionBox *channelsBox;
    WPushButton *subscribeButton;
    WPushButton *unsubscribeButton;

public:
    ChannelsWidget(WContainerWidget *parent = 0);

};


#endif /* CHANNELSWIDGET_H */
