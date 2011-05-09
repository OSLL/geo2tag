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
#include "Control.h"
#include <QPointF>
#include <QNetworkConfigurationManager>

class TrackerDaemon : /*public QThread,*/ public Control
{
  Q_OBJECT;

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

  QTcpServer * m_controlServer;
  public:
    void run();
  private slots:

    //calls when login/pass were accepted
    void onConnected();
    void onTagAdded();
    void onError(QString);

    void onOnlineChanged(bool state);

    void newControlConnection();

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
// TRACKERDAEMON_H
#endif
