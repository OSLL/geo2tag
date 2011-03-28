#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WSignal>
#include "DataMarks.h"
#include <QSharedPointer>
#include "LoginQuery.h"
#include "GoogleMap.h"
#include "LoginWidgetConnector.h"

using namespace Wt;

class LoginWidget : public WContainerWidget
{
   WLineEdit * usernameEdit;
   WLineEdit * passwordEdit;
   WPushButton * loginButton;
   WGoogleMap * map;
   //   Connector<LoginWidget> * m_con;
   LoginWidgetConnector * m_connector;
   QSharedPointer<User> m_user;
   LoginQuery * m_loginQuery;
//   std::string m_token;
   void initCons();
 
public:
    LoginWidget(WContainerWidget *parent = 0);
    void fillMap();

    /* signals */
//    Wt::Signal<std::string> loginSuccessful;

    Wt::Signal<QSharedPointer<User> > loginSuccessful;
    /* slots */
    void loginClicked();
    void userRecieved();
};

#endif // LOGINWIDGET_H
