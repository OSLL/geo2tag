#include "sketch.h"
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
using namespace Wt;

Sketch::Sketch(QSharedPointer<User> user, WContainerWidget *parent):WContainerWidget(parent)
{
  m_user = user;
  menuContainer = new WContainerWidget(this);
  stackedWidget = new WStackedWidget(this);
  prefButton = new WPushButton("Preferences", menuContainer);
  channelsButton = new WPushButton("Channels", menuContainer);
  marksButton = new WPushButton("Marks", menuContainer);
  optionsButton = new WPushButton("Options", menuContainer);
  adminButton = new WPushButton("Users management", menuContainer);
  // channelsWidget = new ChannelsWidget(m_user, stackedWidget);
  marksWidget = new MarksWidget(m_user, stackedWidget);
  // optionsWidget = new OptionsWidget(stackedWidget);
  // usersWidget = new UsersWidget(m_user,stackedWidget);
  // prefWidget = new PrefWidget(m_user, stackedWidget);
  // WSelectionBox *box = new WSelectionBox(root());

  stackedWidget->setCurrentWidget(marksWidget);
}


/* signals and slots */
//    prefButton->clicked().connect(this, &Sketch::onPrefClicked);
//    channelsButton->clicked().connect(this, &Sketch::onChannelsClicked);
//    marksButton->clicked().connect(this, &Sketch::onMarksClicked);
///    optionsButton->clicked().connect(this, &Sketch::onOptionsClicked);
//    adminButton->clicked().connect(this, &Sketch::onAdminClicked);
//    channelsWidget->channelsUpdated.connect(marksWidget, &MarksWidget::updateModel);
//}

/*
void Sketch::onPrefClicked()
{
    stackedWidget->setCurrentWidget(prefWidget);
}

void Sketch::onChannelsClicked()
{
    stackedWidget->setCurrentWidget(channelsWidget);
}*/

void Sketch::onMarksClicked()
{
  stackedWidget->setCurrentWidget(marksWidget);
}


/*
void Sketch::onOptionsClicked()
{
    stackedWidget->setCurrentWidget(optionsWidget);
}

void Sketch::onAdminClicked()
{
    stackedWidget->setCurrentWidget(usersWidget);
}*/
