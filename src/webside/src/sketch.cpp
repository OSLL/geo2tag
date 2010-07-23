
#include "sketch.h"
#include "DbSession.h"

#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>

/*
 * The env argument contains information about the new session, and
 * the initial request. It must be passed to the WApplication
 * constructor so it is typically also an argument for your custom
 * application constructor.
*/
Sketch::Sketch(const std::string &token, WContainerWidget *parent)
    : WContainerWidget(parent)
{
    //setTitle("Geo2tag"); // application title
    // common::DbSession::getInstance();

    m_token = token;
    menuContainer = new WContainerWidget(this);
    stackedWidget = new WStackedWidget(this);
    privateButton = new WPushButton("Private", menuContainer);
    channelsButton = new WPushButton("Channels", menuContainer);
    marksButton = new WPushButton("Marks", menuContainer);
    optionsButton = new WPushButton("Options", menuContainer);
    adminButton = new WPushButton("Administration", menuContainer);
    channelsWidget = new ChannelsWidget(m_token, stackedWidget);
    marksWidget = new MarksWidget(m_token, stackedWidget);
    optionsWidget = new OptionsWidget(stackedWidget);
    //WSelectionBox *box = new WSelectionBox(root());

    stackedWidget->setCurrentWidget(channelsWidget);

    //  /* Setting up menu container */
    //  WHBoxLayout *menuLayout = new WHBoxLayout();
    //  menuLayout->addWidget(privateButton);
    //  menuLayout->addWidget(channelsButton);
    //  menuLayout->addWidget(marksButton);
    //  menuLayout->addWidget(optionsButton);
    //  menuLayout->addWidget(adminButton);
    //  menuContainer->setLayout(menuLayout);

    //  /* Setting up main container */
    //  WVBoxLayout *mainLayout = new WVBoxLayout();
    //  mainLayout->addWidget(menuContainer);
    //  mainLayout->addWidget(stackedWidget);
    //  mainContainer->setLayout(mainLayout, AlignJustify | AlignTop);
    //  //mainContainer->resize(600, 600);

    //  /* Setting up main container as a root */
    //  root()->addWidget(mainContainer);

    /* signals and slots */
    privateButton->clicked().connect(this, &Sketch::onPrivateClicked);
    channelsButton->clicked().connect(this, &Sketch::onChannelsClicked);
    marksButton->clicked().connect(this, &Sketch::onMarksClicked);
    optionsButton->clicked().connect(this, &Sketch::onOptionsClicked);
    adminButton->clicked().connect(this, &Sketch::onAdminClicked);
    channelsWidget->channelsUpdated.connect(marksWidget, &MarksWidget::updateModel);
}

void Sketch::onPrivateClicked()
{
    //stackedWidget->setCurrentWidget(privateWidget);
}

void Sketch::onChannelsClicked()
{
    stackedWidget->setCurrentWidget(channelsWidget);
}

void Sketch::onMarksClicked()
{
    stackedWidget->setCurrentWidget(marksWidget);
}

void Sketch::onOptionsClicked()
{
    stackedWidget->setCurrentWidget(optionsWidget);
}

void Sketch::onAdminClicked()
{
    //stackedWidget->setCurrentWidget(adminWidget);
}

