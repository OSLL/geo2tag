
#include "sketch.h"

#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>

/*
 * The env argument contains information about the new session, and
 * the initial request. It must be passed to the WApplication
 * constructor so it is typically also an argument for your custom
 * application constructor.
*/
Scetch::Scetch(const WEnvironment& env)
  : WApplication(env)
{
  setTitle("Geo2tag");                               // application title
  
  privateButton = new WPushButton("Private", root());
  channelsButton = new WPushButton("Channels", root());
  marksButton = new WPushButton("Marks", root());
  optionsButton = new WPushButton("Options", root());
  adminButton = new WPushButton("Administration", root());
  mainContainer = new WContainerWidget(root());
  menuContainer = new WContainerWidget(root());
  stackedWidget = new WStackedWidget(root());
  channelsWidget = new ChannelsWidget(root());
  marksWidget = new MarksWidget(root());
  optionsWidget = new OptionsWidget(root());
  //WSelectionBox *box = new WSelectionBox(root());

  stackedWidget->addWidget(channelsWidget);
  stackedWidget->addWidget(marksWidget);
  stackedWidget->addWidget(optionsWidget);
  stackedWidget->setCurrentWidget(optionsWidget);

  /* Setting up menu container */
  WHBoxLayout *menuLayout = new WHBoxLayout();
  menuLayout->addWidget(privateButton);
  menuLayout->addWidget(channelsButton);
  menuLayout->addWidget(marksButton);
  menuLayout->addWidget(optionsButton);
  menuLayout->addWidget(adminButton);
  menuContainer->setLayout(menuLayout);

  /* Setting up main container */
  WVBoxLayout *mainLayout = new WVBoxLayout();
  mainLayout->addWidget(menuContainer);
  mainLayout->addWidget(stackedWidget);  
  mainContainer->setLayout(mainLayout, AlignJustify | AlignTop);
  //mainContainer->resize(600, 600);

  /* Setting up main container as a root */
  root()->addWidget(mainContainer);

  /* signals and slots */
  privateButton->clicked().connect(this, &Scetch::onPrivateClicked);
  channelsButton->clicked().connect(this, &Scetch::onChannelsClicked);
  marksButton->clicked().connect(this, &Scetch::onMarksClicked);
  optionsButton->clicked().connect(this, &Scetch::onOptionsClicked);
  adminButton->clicked().connect(this, &Scetch::onAdminClicked);

}

void Scetch::onPrivateClicked()
{
  //stackedWidget->setCurrentWidget(privateWidget);
}

void Scetch::onChannelsClicked()
{
  stackedWidget->setCurrentWidget(channelsWidget);
}

void Scetch::onMarksClicked()
{
  stackedWidget->setCurrentWidget(marksWidget);
}

void Scetch::onOptionsClicked()
{
  stackedWidget->setCurrentWidget(optionsWidget);
}

void Scetch::onAdminClicked()
{
  //stackedWidget->setCurrentWidget(adminWidget);
}

