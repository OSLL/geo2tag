#ifndef MAEMODAEMON_H_
#define MAEMODAEMON_H_

#include "DaemonManager.h"
#include <QTcpSocket>

class MaemoDaemon : public Daemon
{

  QPointF m_lastCoordinates;
  bool m_started;
  QTcpSocket m_daemon;
  public:
    MaemoDaemon();
    virtual QPointF getLastCoordinates() const;
    virtual bool isConnected() const;
    virtual bool isStarted() const;
    virtual void start();
    virtual void stop();
    virtual void reload();
    virtual void restart();
  public slots:
    void readData();

};
#endif
