#ifndef TRACKERDAEMON_H
#define TRACKERDAEMON_H

#include <QObject>
#include <QDateTime>
#include <QPointer>
#include <QTcpServer>
#include <QTimerEvent>
#include <QTcpSocket>
#include "tracker.h"

/* QtMobility API headers */
// Location
//#include <qgeopositioninfosource.h>
//#include <qgeopositioninfo.h>

#include "GpsInfo.h"

#include "ApplyMarkQuery.h"
#include "SubscribeChannelQuery.h"
#include "LoginQuery.h"
#include "ApplyChannelQuery.h"
#include "RequestReceiver.h"

// Bearer
// #include <qnetworkconfigmanager.h>
// #include <qnetworksession.h>

// QtMobility namespace
//QTM_USE_NAMESPACE


class TrackerDaemon : public QObject
{
    Q_OBJECT;

    Settings   m_settings;
    QDateTime  m_lastAttempt;
    QTcpServer *m_server;
    QTcpSocket *m_uiSocket;

    RequestReceiver *m_receiver;

    QString m_status;
    QString m_statusDescription;
    int m_timerID;
    /* Location data
  QPointer<QGeoPositionInfoSource> m_positionSource;
  QGeoPositionInfo m_positionInfo;*/

    GUI::ApplyMarkQuery m_applyMarkQuery;
    GUI::SubscribeChannelQuery m_subscribeQuery;
    GUI::LoginQuery m_loginQuery;
    GUI::ApplyChannelQuery m_applyChannelQuery;
    // for bearer management
    // QNetworkSession* m_session;

public:
    TrackerDaemon();

    void timerEvent(QTimerEvent *te);

protected:
    void initSettings();
    bool setMark();

    //private:
    //  void startGps();

public slots:

    void start();// start sending marks
    void stop();// stop sending marks
    void login(QString login,QString password); // get auth tocken
    void setChannel(QString channel,QString channelKey); // subscribe channel
    void status(); // return m_lastAttempt and m_status m_statusDescription
    void addChannel(QString channel,QString channelKey);

    void setStatus(QString channel,QString channelKey);

    void uiConnected();

    //void processSocketData();
    void cleanLocalSettigns();
    void readSettings();
    void createSettings();
    void setupBearer();

    void onSubscribeChannelResponse(QString status,QString status_description);
    void onLoginResponse(QString status,QString auth_token,QString status_description);
    void onApplyMarkResponse(QString,QString);
    void onApplyChannelResponse(QString,QString);

    //void positionUpdated(QGeoPositionInfo gpsPos);

};

#endif // TRACKERDAEMON_H
