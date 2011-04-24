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

class TrackerDaemon : /*public QThread,*/ public Control
{
    Q_OBJECT;

    QSettings m_settings;
    QString m_channelName;
    QString m_visibleName;
    QPointF m_lastCoords;

    LoginQuery * m_loginQuery; // this field needs because query is asynchronous
    AddNewMarkQuery * m_tagQuery; // this field needs because query is asynchronous

    bool m_pauseFlag;
    bool m_isConnected;

    QTcpServer * m_controlServer;
public:
    void run();
private slots:

    void onConnected(); //calls when login/pass were accepted
    void onTagAdded();
    void onError(QString);

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

#endif // TRACKERDAEMON_H
