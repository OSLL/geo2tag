#ifndef MARKSHISTORY_H
#define MARKSHISTORY_H

#include <QObject>
#include <QVector>
#include "DataMarks.h"

//this class implement vector with fixed size
class MarksHistory : public QObject
{
  Q_OBJECT
    QVector<QSharedPointer<DataMark> > m_marks;
  int m_historyLimit;
  public:
    explicit MarksHistory(QObject *parent = 0);
    void setHistoryLimit(int limit);
    int getHistoryLimit();
    bool isEmpty();
    void pushMark(QSharedPointer<DataMark> mark);
    QSharedPointer<DataMark> popMark();
    signals:
    void isFull();
  public slots:

};
#endif                                  // MARKSHISTORY_H
