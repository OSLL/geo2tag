#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_
#include <QObject>
#include "LoginQuery.h"
#include "RSSFeedQuery.h"
#include <Wt/WObject>
using namespace Wt;

enum sign
{
     LoginQueryConnected,
     RssFeedRecieved,
     Unknown
};

template< class wClass >
class Connector: public QObject
{
	Q_OBJECT;
	
        void (wClass::*m_Function)(void);
        QObject * m_obj;
	WObject * m_wt;
	sign m_action;


public:

	Connector(QObject * obj,sign a,void (wClass::*func)(void),WObject * wt,QObject * parent=0):
						QObject(parent),m_obj(obj),m_Function(func),m_wt(wt),m_action(a)
	{
		decide();
	}

       void decide(){
		switch (m_action){
			case LoginQueryConnected:{ 
				QObject::connect(m_obj,Q_SIGNAL(connected()),this,Q_SLOT(trigerred()));
				break;
			}
			case RssFeedRecieved: {
				QObject::connect(m_obj,Q_SIGNAL(rssFeedReceived()),this,Q_SLOT(trigerred()));
				break;
			}
		}
       }

private Q_SLOTS:

	void triggered()
	{
		dynamic_cast<wClass>(m_wt)->*m_Function();
	}
};


#endif
