#ifndef MAEMODAEMON_H_
#define MAEMODAEMON_H_

#include "DaemonManager.h"
#include <QTcpSocket>
#include <QDebug>
#include <QTextStream>
#include <QStringList>
#include <QObject>

class Daemon;

class MaemoDaemon : public Daemon
{
	Q_OBJECT
  QPointF m_lastCoordinates;
  QTcpSocket * m_daemon;
	QTextStream * m_device;
  bool m_started;
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
