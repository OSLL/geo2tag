#ifndef WEBSIDE_H
#define WEBSIDE_H

#include <Wt/WStackedWidget>
#include <Wt/WContainerWidget>
#include <QSharedPointer>
#include "User.h"
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
    void onLoginSuccess(QSharedPointer<User> user);
    };
#endif                                                      // WEBSIDE_H
