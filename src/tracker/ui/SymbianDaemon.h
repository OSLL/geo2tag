#ifndef SYMBIANDAEMON_H_
#define SYMBIANDAEMON_H_

#include "DaemonManager.h"
#include <QtConcurrentRun>

#include "TrackerDaemon.h"

class SymbianDaemon : public Daemon
{
  TrackerDaemon * m_daemon;
  public:
    SymbianDaemon();
    virtual QPointF getLastCoordinates() const;
    virtual bool isConnected() const;
    virtual bool isStarted() const;
    virtual void start();
    virtual void stop();
    virtual void reload();
    virtual void restart();
};
#endif
