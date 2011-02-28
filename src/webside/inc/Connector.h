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

template<class wClass>
class Connector: public QObject
{
//	Q_OBJECT;
	
        void (wClass::*m_Function)(void);
        QObject * m_obj;
	WObject * m_wt;
	sign m_action;


public:
	Connector(QObject * obj,sign a,void (wClass::*func)(void),WObject * wt,QObject * parent=0);
        void  decide();
private Q_SLOTS:

	void triggered();
	};


#endif
