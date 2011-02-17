#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_
#include <QObject>
#include <Wt/WContainerWidget>
using namespace Wt;

enum sign
{
     LoginQueryConnected,
     Unknown
};
template< class wClass >
class Connector: public QObject
{
	Q_OBJECT;
//	typedef void (wClass::*wtFunction)(void);
        void (wClass::*m_Function)(void);
//	wtFunction m_Function;
        QObject * m_obj;
	WContainerWidget * m_wt;
public:
	Connector(QObject * obj,sign a,void (wClass::*func)(void),WContainerWidget * wt,QObject * parent=0);
	void setWtFunction(void (wClass::*function)(void));

private Q_SLOTS:
	void triggered();
};
#endif
