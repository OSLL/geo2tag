#include "Connector.h"
/*
Connector::Connector(QObject * obj,sign a,wtFunction func,WObject * wt,QObject * parent):QObject(parent),m_obj(obj),m_Function(func),m_wt(wt)
{
	switch (a){
		case LoginQueryConnected: {
			connect(m_obj,Q_SIGNAL(connected()),this,Q_SLOT(trigerred()));
			break;
		}
	}
}

void Connector::setWtFunction(wtFunction function)
{
	m_Function=function;
}

void Connector::triggered()
{
	m_wt->*m_Function();
}*/

template< class wClass >
Connector::Connector(QObject * obj,sign a,void (wClass::*func)(void),WContainerWidget * wt,QObject * parent):QObject(parent),m_obj(obj),m_Function(func),m_wt(wt)
{
	switch (a){
		case LoginQueryConnected: {
			connect(m_obj,Q_SIGNAL(connected()),this,Q_SLOT(trigerred()));
			break;
		}
	}
}

template< class T >
void Connector::setWtFunction(void (wClass::*function)(void))
{
	m_Function=function;
}

template< class wClass >
void Connector::triggered()
{
	dynamic_cast<wClass>(m_wt)->*m_Function();
}
