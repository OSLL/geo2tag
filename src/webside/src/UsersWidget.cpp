#include "inc/UsersWidget.h"

#include <Wt/WText>
#include <Wt/WBreak>
#include <Wt/WLabel>

#include "Time.h"
#include "Handle.h"
#include "Crc.h"
#include "BitTools.h"

UsersWidget::UsersWidget(const std::string &token,WContainerWidget *parent)
: WContainerWidget(parent)
{
  m_token=token;
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
  password->setEchoMode(WLineEdit::Password);
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

  WBreak *break4 = new WBreak(this);
  message = new WText("", this);

  addButton->clicked().connect(this, &UsersWidget::addClicked);
  removeButton->clicked().connect(this, &UsersWidget::removeClicked);
  addUserButton->clicked().connect(this, &UsersWidget::addUserClicked);
  cancelUserButton->clicked().connect(this, &UsersWidget::cancelUserClicked);

  actionWidget->setHidden(true);
  updateUsersBox();
}


void UsersWidget::updateUsersBox()
{
  usersBox->clear();
  QSharedPointer<std::vector<QSharedPointer<common::User> > > users =
    common::DbSession::getInstance().getUsers();
  for (int i = 0; i < users->size(); i++)
  {
    QSharedPointer<loader::User> user = users->at(i).dynamicCast<loader::User>();
    WString login = WString(user->getLogin());
    usersBox->addItem(login);
  }
}


void UsersWidget::addClicked()
{
  actionWidget->setCurrentWidget(addUserWidget);
  actionWidget->setHidden(false);
}


void UsersWidget::removeClicked()
{
  QSharedPointer<std::vector<QSharedPointer<common::User> > > users =
    common::DbSession::getInstance().getUsers();

  std::string userToDelete("");

  if (usersBox->currentText() == WString(""))
  {
    message->setText("User isn't selected");
  }
  else
  {
    userToDelete = usersBox->currentText().toUTF8();
  }

  for (int i = 0; i < users->size(); i++)
  {
    QSharedPointer<loader::User> user = users->at(i).dynamicCast<loader::User>();

    std::string login = user->getLogin();
    if (userToDelete == login)
    {
      if (user->getToken() == m_token)
      {
        break;
      }
      common::DbSession::getInstance().removeUser(user);
      message->setText("");
      updateUsersBox();
      break;
    }
  }
}


void UsersWidget::addUserClicked()
{
  QSharedPointer<std::vector<QSharedPointer<common::User> > > users=
    common::DbSession::getInstance().getUsers();

  if (password->text() != password2->text())
  {
    message->setText("Passwords are different");
    return;
  }

  std::string m_login = login->text().toUTF8();
  std::string m_password = password->text().toUTF8();
  std::string m_token;

  for(std::vector<QSharedPointer<common::User> >::iterator i=users->begin();i!=users->end();i++)
  {
    if((*i).dynamicCast<loader::User>()->getLogin()==m_login)
    {
      message->setText("User alredy exist");
      return;
    }
  }

  std::ostringstream s(""), token("");
  s << "token" << QDateTime::now()
    << m_login
    << m_password
    << QDateTime::now();
  CCrc32 crc;
  unsigned long crc32 = crc.add(s.str().c_str(), s.str().size());
  token << crc32 << m_login;
  crc32 = crc.add(token.str().c_str(), token.str().size());
  BitTools::reverse(&crc32, sizeof(crc32));
  token << crc32;
  m_token = token.str();
  QSharedPointer<loader::User> user =
    makeHandle(new loader::User(m_login, m_password, 0, m_token));

  try
  {
    common::DbSession::getInstance().storeUser(user);
    updateUsersBox();
    message->setText("");
    addUserWidget->setHidden(true);
  }
  catch(const CExceptionSource& e)
  {
    message->setText("Can't add user");
  }
}


void UsersWidget::cancelUserClicked()
{
  actionWidget->setHidden(true);
  message->setText("");
}
