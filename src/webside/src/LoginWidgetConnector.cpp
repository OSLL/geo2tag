#include "LoginWidgetConnector.h"
#include "LoginQuery.h"
#include "LoginWidget.h"
#include "qt_signals_fix.h"


LoginWidgetConnector::LoginWidgetConnector(LoginQuery * obj,LoginWidget * wt,void (LoginWidget::*function)(void),QObject * parent):
                QObject(parent),m_obj(obj),m_loginWidget(wt),m_function(function)
		{
		}

void LoginWidgetConnector::act()
{
        this->connect(m_obj,Q_SIGNAL(connected()),this,Q_SLOT(triggered()));
}

void LoginWidgetConnector::triggered()
{
        //m_loginWidget->m_function();
}
