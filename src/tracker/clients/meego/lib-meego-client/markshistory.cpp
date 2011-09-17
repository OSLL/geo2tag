#include "markshistory.h"

MarksHistory::MarksHistory(QObject *parent) :
    QObject(parent),m_historyLimit(50)
{
}


void MarksHistory::setHistoryLimit(int limit)
{
    if (limit<m_historyLimit)
    {
        //Erase all elements from limit to m_historyLimit
        m_marks.remove(limit-1,m_marks.size()-1);

    }
    m_historyLimit = limit;

}

int MarksHistory::getHistoryLimit()
{
    return m_historyLimit;
}

bool MarksHistory::isEmpty()
{
   if (m_marks.size()==0) return true;
   else return false;
}

void MarksHistory::pushMark(QSharedPointer<DataMark> mark)
{
    // if vector size < m_historyLimit, than add new mark
    // if it is equal than emit isFull
    if (m_marks.size()<m_historyLimit)  m_marks.push_front(mark);

    if (m_marks.size()==m_historyLimit) emit isFull();
}

QSharedPointer<DataMark> MarksHistory::popMark()
{
    //return and remove the last element from vector
    QSharedPointer<DataMark> mark(m_marks.at(m_marks.size()-1));
    return mark;
}

