#include "ChannelsWidget.h"
#include "DbSession.h"
#include "defines.h"
#include "UserInternal.h"

#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WContainerWidget>
#include <Wt/WMessageBox>
#include <Wt/WBreak>

ChannelsWidget::ChannelsWidget(const QSharedPointer<User> user, WContainerWidget *parent)
    : WContainerWidget(parent)
{
    m_user=user;
    channelsText = new WText("Available channels:", this);
    WBreak *break1 = new WBreak(this);
    channelsTable = new WTableView(this);
    channelsTable->setMinimumSize(WLength(300), WLength(400));
    WBreak *break2 = new WBreak(this);
    /* create a table model*/
    channelsModel = new ChannelsModel(m_user, channelsTable->parent());
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
    channelsModel = new ChannelsModel(m_user, channelsTable->parent());
    channelsModel->channelsUpdated.connect(this, &ChannelsWidget::onModelUpdated);
    channelsTable->setModel(channelsModel);
    if (oldModel != 0)
        delete oldModel;

}

void ChannelsWidget::onModelUpdated()
{
    this->channelsUpdated.emit();
}

