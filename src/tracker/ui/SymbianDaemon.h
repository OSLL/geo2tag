#ifndef SYMBIANDAEMON_H_
#define SYMBIANDAEMON_H_

#include "DaemonManager.h"
#include "TrackerDaemon.h"
class SymbianDaemon : public Daemon
{

  public:
    virtual QPointF getLastCoordinates() const;
    virtual bool isConnected() const;
    virtual bool isStarted() const;
    virtual void start();
    virtual void stop();
    virtual void reload();
    virtual void restart();
};
#endif
