#ifndef _LOGINWIDGETCONNECTOR_H_
#define _LOGINWIDGETCONNECTOR_H_
#include <QObject>
#include "LoginQuery.h"
#include <Wt/WObject>
using namespace Wt;

class LoginWidget;

class LoginWidgetConnector:public QObject
{
  Q_OBJECT
    LoginQuery * m_obj;
  LoginWidget * m_loginWidget;
  void (LoginWidget::*m_function)(void);
  public:
    LoginWidgetConnector(LoginQuery * obj,LoginWidget * wt,void (LoginWidget::*function)(void),QObject * parent=0);
    void act();
    Q_SIGNALS:
    void test();

  public Q_SLOTS:
    void triggered();
    };
#endif
