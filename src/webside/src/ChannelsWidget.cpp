#include "ChannelsWidget.h"
#include "DbSession.h"
#include "defines.h"
#include "UserInternal.h"

#include <WHBoxLayout>
#include <WVBoxLayout>
#include <WContainerWidget>
#include <WMessageBox>

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

    /* signals and slots */
    subscribeButton->clicked().connect(this, &ChannelsWidget::onSubscribeClicked);
    unsubscribeButton->clicked().connect(this, &ChannelsWidget::onUnsubscribeClicked);

    updateChannelsBox();
}

void ChannelsWidget::updateChannelsBox()
{
    m_availableChannels = common::DbSession::getInstance().getChannels();

    if (m_availableChannels->size() > 0)
        channelsBox->clear();

    /* Looking for subscribed channels */
    CHandlePtr<std::vector<CHandlePtr<common::User> > > users=common::DbSession::getInstance().getUsers();
    for (int i = 0; i < users->size(); i++)
    {
        CHandlePtr<loader::User> user = users->at(i).dynamicCast<loader::User>();
        WString token = WString(user->getToken());
        //channelsBox->addItem(token);
        if (token == WString(DEFAULT_TOKEN))
        {
            //channelsBox->addItem(WString("found"));
            m_subscribedChannels = user->getSubscribedChannels();
        }
    }

    /* Adding channels to Box */
    for (int i = 0; i < m_availableChannels->size(); i++)
    {
        int found = 0;
        for (int j = 0; j < m_subscribedChannels->size(); j++)
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

void ChannelsWidget::onSubscribeClicked()
{
    WString m_channel = channelsBox->currentText();
    if (m_channel != WString(""))
    {
        /* subscribe to channel */
        CHandlePtr<common::Channels > channels=common::DbSession::getInstance().getChannels();
        CHandlePtr<common::User> du;
        CHandlePtr<common::Channel> ch;

        if(common::DbSession::getInstance().getTokensMap().count(std::string(DEFAULT_TOKEN)) == 0)
        {
            WMessageBox::show("Geo2tag", "Can't find user. Ask admin.", Ok);
            return;
        }
        else
        {
            du = common::DbSession::getInstance().getTokensMap().find(std::string(DEFAULT_TOKEN))->second;
        }

        for(size_t i=0; i< channels->size(); ++i)
        {
            if(WString((*channels)[i]->getName()) == m_channel)
            {
                ch = (*channels)[i];
                break;
            }
        }

        if(ch == NULL) // Channel was not found
        {
            WMessageBox::show("Geo2tag", "Can't find channel. Ask admin.", Ok);
            return;
        }

        try
        {
            common::DbSession::getInstance().subscribe(du,ch);
        }
        catch(const CExceptionSource& x)
        {
            WMessageBox::show("Geo2tag", "Exception while common::DbSession::getInstance().subscribe(du,ch). Ask admin", Ok);
        }

        updateChannelsBox();
    }
    else
    {
        WMessageBox::show("Geo2tag", "Nothing is selected", Ok);
    }

}

void ChannelsWidget::onUnsubscribeClicked()
{
    if (channelsBox->currentText() != WString(""))
    {
        /* unsubscribe from channel */

    }
}
