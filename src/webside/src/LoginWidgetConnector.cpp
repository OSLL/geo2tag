#include "LoginWidgetConnector.h"
#include "LoginQuery.h"

LoginWidgetConnector::LoginWidgetConnector(LoginQuery * obj,LoginWidget * wt,void (LoginWidget::*function)(void),QObject * parent):
		QObject(parent),m_obj(obj),m_loginWidget(wt),m_function(function)
		{
		}

void LoginWidgetConnector::act()
{
	this->connect(m_obj,Q_SIGNAL(connected()),this,Q_SLOT(triggered()));
//	this->connect(this,Q_SIGNAL(test()),this,Q_SLOT(triggered()));
}

void LoginWidgetConnector::triggered()
{
	this->m_loginWidget->*m_function();
}
