#include "MarksModelConnector.h"
#include "MarksModel.h"
#include "qt_signals_fix.h"

MarksModelConnector::MarksModelConnector(QObject * obj,MarksModel * wt,void (MarksModel::*function)(void),QObject * parent):
                QObject(parent),m_obj(obj),m_marksModel(wt),m_function(function)
		{
			connect(m_obj,Q_SIGNAL(rssFeedReceived()),this,Q_SLOT(triggered()));
		}

void MarksModelConnector::triggered()
{
        //todo m_function();
}
