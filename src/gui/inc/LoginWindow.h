#ifndef _LoginWindow_h_Included
#define _LoginWindow_h_Included

#include <QMainWindow>

#include "LoginQuery.h"
#include "ui_LoginForm.h"

namespace  GUI
{
  class LoginWindow: public QMainWindow
  {
    Q_OBJECT;
    Ui::LoginForm m_form;
    LoginQuery m_query;

    public:
      LoginWindow(QWidget * parent = 0, Qt::WindowFlags f = 0);

    private slots:
      void okPressed();
      void responseReceived(QString status, QString auth_token,QString status_description);
      void errorReceived();

      signals:
      void onSucsess(QString authToken);
  };
}
#endif
