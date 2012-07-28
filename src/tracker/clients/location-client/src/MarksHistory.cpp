#include "MarksHistory.h"

#include <QDebug>

#define DEFAULT_CAPACITY 50

MarksHistory::MarksHistory(QObject *parent) :
QObject(parent),m_historyLimit(DEFAULT_CAPACITY)
{
}


void MarksHistory::setHistoryLimit(int limit)
{
  if (limit<m_historyLimit)
  {
    //Erase all elements from limit to m_historyLimit
    if (limit<m_marks.size()) m_marks.remove(limit,m_marks.size()-limit);

  }
  m_historyLimit = limit;

}


int MarksHistory::getHistoryLimit()
{
  return m_historyLimit;
}


bool MarksHistory::isEmpty()
{
  if (m_marks.size()==0)
    return true;
  else return false;
}


void MarksHistory::pushMark(QSharedPointer<DataMark> mark)
{
  // if vector size < m_historyLimit, than add new mark
  // if it is equal than emit isFull
  qDebug() << "Pushing mark into vector. In vector " <<m_marks.size()+1 <<", limit "<<m_historyLimit ;
  m_marks.push_front(mark);
  if (m_marks.size()==m_historyLimit+1)
  {
    emit isFull();
    //   m_marks.pop_back();
  }
  QString tmp="Mark history:";
  for (int i=0;i<m_marks.size();i++)
  {
    tmp+=QString("%1;%2 ").arg(QString::number(m_marks.at(i)->getLatitude())).arg(QString::number(m_marks.at(i)->getLongitude()));
  }
  qDebug() << tmp;
}


QSharedPointer<DataMark> MarksHistory::popMark()
{
  //return and remove the last element from vector
  qDebug() << "Accessing to "<< m_marks.size();
  QSharedPointer<DataMark> mark(m_marks.at(m_marks.size()-1));
  m_marks.remove(m_marks.size()-1);
  return mark;
}
