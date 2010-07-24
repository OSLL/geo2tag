#ifndef USERSWIDGET_H
#define USERSWIDGET_H

#include <Wt/WContainerWidget>
#include <WSelectionBox>
#include <WPushButton>
#include <WLineEdit>
#include <WStackedWidget>
#include <WTable>

using namespace Wt;

class UsersWidget : public WContainerWidget
{
    WSelectionBox *usersBox;
    WPushButton *addButton;
    WPushButton *removeButton;
    WStackedWidget *actionWidget; /* contains widget for concrete action
                                     with usersBox, for example widget
                                     for input inforamtion about new
                                     user */

    /* User information widget */
    WTable *addUserWidget;
    WLineEdit *login;
    WLineEdit *password;
    WLineEdit *password2;
    WPushButton *addUserButton;
    WPushButton *cancelUserButton;


public:
    UsersWidget(WContainerWidget *parent = 0);

    void updateUsersBox();

    /* Slots */
    void addClicked();
    void removeClicked();
    void addUserClicked();
    void cancelUserClicked();

};

#endif // USERSWIDGET_H
