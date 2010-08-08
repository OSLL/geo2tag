#ifndef WEBSIDE_H
#define WEBSIDE_H

#include <Wt/WStackedWidget>
#include <WContainerWidget>

#include "LoginWidget.h"
#include "sketch.h"

using namespace Wt;

class Webside : public WContainerWidget
{
    WStackedWidget *stackedWidget;
    LoginWidget *loginWidget;
    Sketch *mainWidget;

public:
    Webside(WContainerWidget *parent = 0);

    /* slots */
    void onLoginSuccess(std::string token);
};

#endif // WEBSIDE_H
