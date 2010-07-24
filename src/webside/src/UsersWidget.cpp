#include "inc/UsersWidget.h"

#include <WText>
#include <WBreak>
#include <WLabel>

UsersWidget::UsersWidget(WContainerWidget *parent)
    : WContainerWidget(parent)
{
    WText *usersText = new WText("Users:", this);
    WBreak *break1 = new WBreak(this);
    usersBox = new WSelectionBox(this);
    usersBox->setMinimumSize(WLength(300), WLength(300));
    WBreak *break2 = new WBreak(this);
    addButton = new WPushButton("Add", this);
    removeButton = new WPushButton("Remove", this);
    WBreak *break3 = new WBreak(this);

    actionWidget = new WStackedWidget(this);

    addUserWidget = new WTable(actionWidget);
    WLabel *loginLabel = new WLabel("Login:",
                                    addUserWidget->elementAt(0, 0));
    login = new WLineEdit(addUserWidget->elementAt(0, 1));
    loginLabel->setBuddy(login);
    WLabel *passwordLabel = new WLabel("Password:",
                                       addUserWidget->elementAt(1, 0));
    password = new WLineEdit(addUserWidget->elementAt(1, 1));
    passwordLabel->setBuddy(password);
    WLabel *password2Label = new WLabel("Confirm password:",
                                        addUserWidget->elementAt(2, 0));
    password2 = new WLineEdit(addUserWidget->elementAt(2, 1));
    password2->setEchoMode(WLineEdit::Password);
    password2Label->setBuddy(password);
    addUserButton = new WPushButton("Add user",
                                    addUserWidget->elementAt(3, 0));
    cancelUserButton = new WPushButton("Cancel",
                                       addUserWidget->elementAt(3, 1));

    addButton->clicked().connect(this, &UsersWidget::addClicked);
    removeButton->clicked().connect(this, &UsersWidget::removeClicked);
    addUserButton->clicked().connect(this, &UsersWidget::addUserClicked);
    cancelUserButton->clicked().connect(this, &UsersWidget::cancelUserClicked);

    actionWidget->setHidden(true);
    updateUsersBox();
}

void UsersWidget::updateUsersBox()
{

}

void UsersWidget::addClicked()
{
    actionWidget->setCurrentWidget(addUserWidget);
    actionWidget->setHidden(false);
}

void UsersWidget::removeClicked()
{

}

void UsersWidget::addUserClicked()
{

}

void UsersWidget::cancelUserClicked()
{
    actionWidget->setHidden(true);
}
