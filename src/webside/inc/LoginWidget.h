#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <Wt/WContainerWidget>
#include <WLineEdit>
#include <WPushButton>

#include "GoogleMap.h"

using namespace Wt;

class LoginWidget : public WContainerWidget
{
   WLineEdit *usernameEdit;
   WLineEdit *passwordEdit;
   WPushButton *loginButton;
   WGoogleMap *map;

   std::string m_token;

public:
    LoginWidget(WContainerWidget *parent = 0);
    void fillMap();

    /* signals */
    Wt::Signal<std::string> loginSuccessful;

    /* slots */
    void loginClicked();
};

#endif // LOGINWIDGET_H
