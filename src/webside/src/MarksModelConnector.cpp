#include "MarksModelConnector.h"

MarksModelConnector::MarksModelConnector(QObject * obj,MarksModel * wt,void (MarksModel::*function)(void),QObject * parent):
		QObject(parent),m_obj(obj),m_function(function),m_marksModel(wt)
		{
			connect(m_obj,Q_SIGNAL(rssFeedReceived()),this,Q_SLOT(triggered()));
		}

void MarksModelConnector::triggered()
{
	m_marksModel->*m_function();
}
