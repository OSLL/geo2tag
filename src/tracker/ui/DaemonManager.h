#ifndef DAEMONMANAGER_H_
#define DAEMONMANAGER_H_
#include <QObject>
#include <QPointF>


class Daemon: public QObject
{
				Q_OBJECT
  public:
		Daemon():QObject(0){}
    virtual QPointF getLastCoordinates() const=0;
    virtual bool isConnected() const=0;
    virtual bool isStarted() const=0;
    virtual void start() =0;
    virtual void stop() =0;
    virtual void reload() =0;
    virtual void restart() =0;
};

class DaemonManager
{

  public:
    DaemonManager();
    ~DaemonManager();

    static Daemon& getInstance();

};
// DAEMONMANAGER_H
#endif
