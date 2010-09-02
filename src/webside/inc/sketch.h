#ifndef SKETCH_H
#define SKETCH_H

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include <Wt/WStackedWidget>
#include <Wt/WContainerWidget>

#include "PrefWidget.h"
#include "ChannelsWidget.h"
#include "MarksWidget.h"
#include "OptionsWidget.h"
#include "UsersWidget.h"

using namespace Wt;

/*
 * A sketch for futher webside application.
 */
class Sketch : public WContainerWidget
{
public:
    Sketch(const std::string &token, WContainerWidget *parent = 0);

private:
    std::string m_token;

    WPushButton *prefButton;
    WPushButton *channelsButton;
    WPushButton *marksButton;
    WPushButton *optionsButton;
    WPushButton *adminButton;

    WContainerWidget *mainContainer;
    WContainerWidget *menuContainer;
    WStackedWidget *stackedWidget;

    PrefWidget *prefWidget;
    ChannelsWidget *channelsWidget;
    MarksWidget *marksWidget;
    OptionsWidget *optionsWidget;
    UsersWidget *usersWidget;

    /* slots */
    void onPrefClicked();
    void onChannelsClicked();
    void onMarksClicked();
    void onOptionsClicked();
    void onAdminClicked();

};

#endif /* SKETCH_H */
