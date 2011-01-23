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
#include "RSSFeedQuery.h"
using namespace Wt;

class LoginWidget : public WContainerWidget,QObject
{
	Q_OBJECT;
   WLineEdit *usernameEdit;
   WLineEdit *passwordEdit;
   WPushButton *loginButton;
   WGoogleMap *map;

   QSharedPointer<User> m_user;
   LoginQuery loginQuery;
//   std::string m_token;

public slots:

   void userRecieved();
 
public:
    LoginWidget(WContainerWidget *parent = 0);
    void fillMap();

    /* signals */
//    Wt::Signal<std::string> loginSuccessful;

    Wt::Signal<QSharedPointer<User> > loginSuccessful;
    /* slots */
    void loginClicked();
};

#endif // LOGINWIDGET_H
