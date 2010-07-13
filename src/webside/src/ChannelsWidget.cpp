#include "ChannelsWidget.h"

#include <WHBoxLayout>
#include <WVBoxLayout>
#include <WContainerWidget>

ChannelsWidget::ChannelsWidget(WContainerWidget *parent)
    : WContainerWidget(parent)
{
    channelsText = new WText("Available channels:", parent);
    channelsBox = new WSelectionBox(parent);
    subscribeButton = new WPushButton("Subscribe", parent);
    unsubscribeButton = new WPushButton("Unsubscribe", parent);

    /* Setting up buttons layout */
    WHBoxLayout *buttonsLayout = new WHBoxLayout();
    buttonsLayout->addWidget(subscribeButton);
    buttonsLayout->addWidget(unsubscribeButton);
    
    /* Setting up channels widget main container */
    WVBoxLayout *mainLayout = new WVBoxLayout();
    mainLayout->addWidget(channelsText);
    mainLayout->addWidget(channelsBox);
    mainLayout->addItem(buttonsLayout);
    this->setLayout(mainLayout);
}
