#ifndef _MARKSMODELCONNECTOR_H_
#define _MARKSMODELCONNECTOR_H_
#include <QObject>
#include "RSSFeedQuery.h"
#include <Wt/WObject>
using namespace Wt;

class MarksModel;

class MarksModelConnector:public QObject
{
  Q_OBJECT;
  QObject * m_obj;
  MarksModel * m_marksModel;
  void (MarksModel::*m_function)(void);
  public:
    MarksModelConnector(QObject * obj,MarksModel * wt,void (MarksModel::*function)(void),QObject * parent=0);

  public Q_SLOTS:
    void triggered();

};
#endif
