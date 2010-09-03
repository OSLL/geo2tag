#include "ChannelsWidget.h"
#include "DbSession.h"
#include "defines.h"
#include "UserInternal.h"

#include <WHBoxLayout>
#include <WVBoxLayout>
#include <WContainerWidget>
#include <WMessageBox>
#include <WBreak>

ChannelsWidget::ChannelsWidget(const std::string &token, WContainerWidget *parent)
    : WContainerWidget(parent)
{
    m_token = token;
    channelsText = new WText("Available channels:", this);
    WBreak *break1 = new WBreak(this);
    channelsTable = new WTableView(this);
    channelsTable->setMinimumSize(WLength(300), WLength(400));
    WBreak *break2 = new WBreak(this);
    subscribeButton = new WPushButton("Subscribe", this);
    unsubscribeButton = new WPushButton("Unsubscribe", this);
    /* signals and slots */
    subscribeButton->clicked().connect(this, &ChannelsWidget::onSubscribeClicked);
    unsubscribeButton->clicked().connect(this, &ChannelsWidget::onUnsubscribeClicked);
    /* create a table model*/
    channelsModel = new ChannelsModel(m_token, channelsTable->parent());
    channelsModel->channelsUpdated.connect(this, &ChannelsWidget::onModelUpdated);
    channelsTable->setModel(channelsModel);
    channelsTable->setSelectionMode(ExtendedSelection);
    channelsTable->setColumnAlignment(2, Wt::AlignCenter);
    channelsTable->setColumnAlignment(3, Wt::AlignCenter);
    channelsTable->setSelectable(true);
//    channelsTable->setSelectionBehavior(SelectItems);
//    updateChannelsModel();
}

void ChannelsWidget::updateChannelsModel()
{
    ChannelsModel *oldModel = channelsModel;
    channelsModel = new ChannelsModel(m_token, channelsTable->parent());
    channelsModel->channelsUpdated.connect(this, &ChannelsWidget::onModelUpdated);
    channelsTable->setModel(channelsModel);
    if (oldModel != 0)
        delete oldModel;

}

void ChannelsWidget::onModelUpdated()
{
    this->channelsUpdated.emit();
}

void ChannelsWidget::onSubscribeClicked()
{/*
//    WString m_channel = channelsTable->currentText();
#ifdef WT_THREADED
    WMessageBox::show("Geo2tag", "hello", Ok);
#endif
    if (m_channel != WString(""))
    {
      //  CHandlePtr<common::Channels > channels=common::DbSession::getInstance().getChannels();
        CHandlePtr<common::User> du;
        CHandlePtr<common::Channel> ch;

        if(common::DbSession::getInstance().getTokensMap().count(std::string(m_token)) == 0)
        {
#ifdef WT_THREADED
            WMessageBox::show("Geo2tag", "Can't find user. Ask admin.", Ok);
#else
//    channelsTable->addItem(WString("Can't find user"));
#endif
            return;
        }
        else
        {
            du = common::DbSession::getInstance().getTokensMap().find(std::string(m_token))->second;
        }

	int i=channelsTable->selectedIndexes();
	ch = channelsModel->getAvailableChannels()->at(channelsTable->currentRow());

        if(ch == NULL) // Channel was not found
        {
#ifdef WT_THREADED
            WMessageBox::show("Geo2tag", "Can't find channel. Ask admin.", Ok);
#else
//    channelsTable->addItem(WString("Can't find channel"));
#endif
            return;
        }

        try
        {
            common::DbSession::getInstance().subscribe(du,ch);
        }
        catch(const CExceptionSource& x)
        {
#ifdef WT_THREADED
            WMessageBox::show("Geo2tag", "Exception while common::DbSession::getInstance().subscribe(du,ch). Ask admin", Ok);
#else
//    channelsTable->addItem(WString("Can't subscribe"));
#endif
        }

        updateChannelsModel();
        channelsUpdated.emit();
    }
    else
    {
#ifdef WT_THREADED
        WMessageBox::show("Geo2tag", "Nothing is selected", Ok);
#else
  //  channelsTable->addItem(WString("Nothing is selected"));
#endif
    }*/

}

void ChannelsWidget::onUnsubscribeClicked()
{/*
//WString m_channel = channelsTable->currentText();
#ifdef WT_THREADED
    WMessageBox::show("Geo2tag", "hello", Ok);
#endif
    if (m_channel != WString(""))
    {
    //    CHandlePtr<common::Channels > channels=common::DbSession::getInstance().getChannels();
        CHandlePtr<common::User> du;
        CHandlePtr<common::Channel> ch;

        if(common::DbSession::getInstance().getTokensMap().count(std::string(m_token)) == 0)
        {
#ifdef WT_THREADED
            WMessageBox::show("Geo2tag", "Can't find user. Ask admin.", Ok);
#else
   // channelsTable->addItem(WString("Can't find user"));
#endif
            return;
        }
        else
        {
            du = common::DbSession::getInstance().getTokensMap().find(std::string(m_token))->second;
        }

	ch = channelsModel->getAvailableChannels()->at(channelsTable->currentRow());
        if(ch == NULL) // Channel was not found
        {
#ifdef WT_THREADED
            WMessageBox::show("Geo2tag", "Can't find channel. Ask admin.", Ok);
#else
    //channelsTable->addItem(WString("Can't find channel"));
#endif
            return;
        }

        try
        {
            common::DbSession::getInstance().unsubscribe(du,ch);
        }
        catch(const CExceptionSource& x)
        {
#ifdef WT_THREADED
            WMessageBox::show("Geo2tag", "Exception while common::DbSession::getInstance().subscribe(du,ch). Ask admin", Ok);
#else
  //  channelsTable->addItem(WString("Can't unsubscribe"));
#endif
        }

        updateChannelsModel();
        channelsUpdated.emit();
    }
    else
    {
#ifdef WT_THREADED
        WMessageBox::show("Geo2tag", "Nothing is selected", Ok);
#else
//    channelsTable->addItem(WString("Nothing is selected"));
#endif
*/
    }

