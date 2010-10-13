#include "inc/LoginWidget.h"

#include "DbSession.h"
#include "UserInternal.h"
#include "DataMarks.h"
#include "Channel.h"
#include "defines.h"
#include "OpenStreetMap.h"
#include "YandexMap.h"

#include <Wt/WLabel>
#include <Wt/WText>
#include <Wt/WBreak>

LoginWidget::LoginWidget(WContainerWidget *parent)
    : WContainerWidget(parent)
{
    this->setId("login_widget");
    this->setStyleClass("login_widget_style");
    WText *intro = new WText("Hello! Please, login into Geo2tag "
                             "or view marks from public channels",
                             this);
    intro->setId("intro");
    WBreak *break1 = new WBreak(this);
    WLabel *usernameLabel = new WLabel("Username:", this);
    usernameEdit = new WLineEdit(this);
    usernameLabel->setBuddy(usernameEdit);
    WLabel *passwordLabel = new WLabel("Password:", this);
    passwordEdit = new WLineEdit(this);
    passwordEdit->setEchoMode(WLineEdit::Password);
    passwordLabel->setBuddy(passwordEdit);
    loginButton = new WPushButton("Login", this);
    WBreak *break2 = new WBreak(this);
    map = new WGoogleMap(this);
    map->setMinimumSize(WLength(300), WLength(400));
    map->setMaximumSize(WLength(500), WLength(400));
    map->setCenter(Wt::WGoogleMap::Coordinate(60, 30));
    map->enableScrollWheelZoom();

//    WBreak *break3 = new WBreak(this);
//    WOpenStreetMap *map2 = new WOpenStreetMap(this);
//    map2->setMinimumSize(WLength(300), WLength(400));
//    //map2->setMaximumSize(WLength(500), WLength(400));
//
//    WBreak *break4 = new WBreak(this);
//    WYandexMap *map3 = new WYandexMap(this);
//    map3->setMinimumSize(WLength(300), WLength(400));

    loginButton->clicked().connect(this, &LoginWidget::loginClicked);

    fillMap();

    //this->setStyleClass("login_wigdet");
}

void LoginWidget::fillMap()
{
    CHandlePtr<common::DataMarks> marks = common::DbSession::
                                          getInstance().getMarks();
    map->clearOverlays();
    for (int i = 0; i < marks->size(); i++)
    {
        map->addMarker(WGoogleMap::
                       Coordinate(marks->at(i)->getLatitude(),
                                  marks->at(i)->getLongitude()),
                       marks->at(i)->getDescription());
    }
}

void LoginWidget::loginClicked()
{
    std::string name = usernameEdit->text().toUTF8();
    std::string pass = passwordEdit->text().toUTF8();
    CHandlePtr<std::vector<CHandlePtr<common::User> > > users =
            common::DbSession::getInstance().getUsers();
    for (int i = 0; i < users->size(); i++)
    {
        if ((users->at(i)->getLogin() == name) &&
            (users->at(i)->getPassword() == pass))
        {
            m_token = users->at(i).dynamicCast<loader::User>()->getToken();
            loginSuccessful.emit(m_token);
            break;
        }
    }

    /* For testing without db */
    if (users->size() == 0)
    {
        loginSuccessful.emit(std::string(DEFAULT_TOKEN));
    }
}
