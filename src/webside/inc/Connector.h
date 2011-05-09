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
  RssFeedReceived
};

template<class wClass>
class Connector: public QObject
{
  Q_OBJECT;

  void (wClass::*m_Function)(void);
  QObject * m_obj;
  wClass * m_wt;
  //	WObject * m_wt;
  sign m_action;

  public:
    Connector(QObject * obj,sign a,void (wClass::*func)(void),wClass * wt,QObject * parent=0):QObject(parent),m_obj(obj),m_wt(wt),m_Function(func)
    {
      switch (a)
      {
        case LoginQueryConnected:
        {
          QObject::connect(this->m_obj,Q_SIGNAL(connected()),this,Q_SLOT(trigerred()));
          break;
        }
        case RssFeedReceived:
        {
          //				connect(m_obj,Q_SIGNAL(rssFeedReceived()),this,Q_SLOT(trigerred()));
          break;
        }
        default:
        {
          break;
        }
      }
    }
  public Q_SLOTS:

    void triggered()
    {
      //		dynamic_cast<wClass>(this->m_wt)->*m_Function();
      this->m_wt->*m_Function();
    }
};
#endif
