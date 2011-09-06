#ifndef TRACKERDAEMON_H
#define TRACKERDAEMON_H

#include <QObject>
#include <QDateTime>
#include <QThread>
#include <QSettings>
#include <QTextStream>
#include <QFile>
#include "GpsInfo.h"
#include "tracker.h"
#include "QTcpServer"
#include "LoginQuery.h"
#include "AddNewMarkQuery.h"
#include <QTimer>
#include <QPointF>
#include <QNetworkConfigurationManager>



#ifndef Q_WS_SYMBIAN
#include "Control.h"
#define PARENT Control
#else
#include <QThread>
#define PARENT QThread
#endif

class TrackerDaemon : /*public QThread,*/ public PARENT
{
  Q_OBJECT

  QSettings m_settings;
  QString m_channelName;
  QString m_visibleName;
  QPointF m_lastCoords;
  QNetworkConfigurationManager m_netManager;

  // this field needs because query is asynchronous
  LoginQuery * m_loginQuery;
  // this field needs because query is asynchronous
  AddNewMarkQuery * m_tagQuery;

  bool m_pauseFlag;
  bool m_isConnected;
  #ifndef NO_DAEMON
  QTcpServer * m_controlServer;
  #endif
  public:
    void run();
  private slots:

    //calls when login/pass were accepted
    void onConnected();
    void onTagAdded();
    void onError(QString);

    void onOnlineChanged(bool state);
  #ifndef NO_DAEMON
    void newControlConnection();
  #endif
  public:
    TrackerDaemon();

    virtual void stopTracking();
    virtual void startTracking();
    virtual void reloadSettings();
    virtual bool isTracking() const;
    const QPointF& getLastCoords() const;

    QStringList getLog() const;

    ~TrackerDaemon();

  public slots:

};
//#endif

// TRACKERDAEMON_H
#endif
