#include "ChannelsWidget.h"
#include "DbSession.h"
#include "defines.h"
#include "UserInternal.h"

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
    m_availableChannels = makeHandle(new common::Channels);
    m_subscribedChannels = makeHandle(new common::Channels);

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

    updateChannelsBox();
}

void ChannelsWidget::updateChannelsBox()
{
    m_availableChannels = common::DbSession::getInstance().getChannels();

    if (m_availableChannels->size() > 0)
        channelsBox->addItem(WString("availables"));

    /* Looking for subscribed channels */
    CHandlePtr<std::vector<CHandlePtr<common::User> > > users=common::DbSession::getInstance().getUsers();
    for (std::vector<CHandlePtr<common::User> >::iterator i=users->begin();i!=users->end();i++)
    {
        if ((*i).dynamicCast<loader::User>()->getToken() == DEFAULT_TOKEN) /* TODO!!! auth-n!!! */
        {
            m_subscribedChannels=(*i)->getSubscribedChannels();
            break;
        }
    }

    if (m_availableChannels->size() > 0)
        channelsBox->addItem(WString("availables2"));

    if (m_subscribedChannels->size() > 0)
        channelsBox->addItem(WString("subscribed"));

    /* Adding channels to Box */
    for (int i = 0; i < m_availableChannels->size(); i++)
    {
        int found = 0;
        for (int j = 0; j < m_subscribedChannels->size(); i++)
        {
            if (m_availableChannels->at(i)->getName() == m_subscribedChannels->at(j)->getName())
            {
                found = 1;
                break;
            }
        }
        if (found)
        {
            /* add channel name in box as subscribed channel */
            channelsBox->addItem(WString(m_availableChannels->at(i)->getName()) + WString(" (subscribed)"));
        }
        else
        {
            /* add as not subscribed channel */
            channelsBox->addItem(WString(m_availableChannels->at(i)->getName()) + WString(" (not subscribed)"));
        }
    }
}
