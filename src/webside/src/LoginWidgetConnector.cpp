#include "LoginWidgetConnector.h"
#include "LoginQuery.h"
#include "LoginWidget.h"
#include "qt_signals_fix.h"
#include <syslog.h>

LoginWidgetConnector::LoginWidgetConnector(LoginQuery * obj,LoginWidget * wt,void (LoginWidget::*function)(void),QObject * parent):
QObject(parent),m_obj(obj),m_loginWidget(wt),m_function(function)
{
  act();
}


void LoginWidgetConnector::act()
{
  this->connect(m_obj,Q_SIGNAL(connected()),this,Q_SLOT(triggered()));
  syslog(LOG_INFO,"just connect slot with signal in LoginWidgetConnector");
}


void LoginWidgetConnector::triggered()
{
  syslog(LOG_INFO,"signal gotten into LoginWidgetConnector");
  (m_loginWidget->*m_function)();
}
