#include "inc/PrefWidget.h"

#include <Wt/WText>
#include <Wt/WBreak>
#include <string.h>
#include "DbSession.h"
#include "Handle.h"
#include "UserInternal.h"

PrefWidget::PrefWidget(const std::string token,
                       WContainerWidget *parent) :
        WContainerWidget(parent),
        m_token(token)
{
    WText *pasText = new WText("Change password:", this);
    WBreak *break1 = new WBreak(this);
    password1 = new WLineEdit(this);
    password1->setEchoMode(WLineEdit::Password);
    password2 = new WLineEdit(this);
    password2->setEchoMode(WLineEdit::Password);
    changeButton = new WPushButton("Change password", this);
    break1 = new WBreak(this);
    errorText = new WText("Wrong passwords!", this);
    errorText->hide();

    changeButton->clicked().connect(this, &PrefWidget::onChangeClicked);
}

void PrefWidget::onChangeClicked()
{
    std::string pass1(password1->text().toUTF8());
    std::string pass2(password2->text().toUTF8());
    if (pass1 != pass2)
    {
        errorText->show();
    }
    else
    {
        errorText->hide();

        QSharedPointer<std::vector<QSharedPointer<common::User> > > users =
                common::DbSession::getInstance().getUsers();

        for (int i = 0; i < users->size(); i++)
        {
            QSharedPointer<loader::User> user = users->at(i).
                                            dynamicCast<loader::User>();

            std::string token = user->getToken();
            if (m_token == token)
            {
                user->setPassword(pass1);
                common::DbSession::getInstance().storeUser(user);
                break;
            }
        }
        password1->setText("");
        password2->setText("");
    }
}
