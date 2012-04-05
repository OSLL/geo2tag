#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QNetworkConfigurationManager>
#include <QTimer>
#include <QMap>

#include "LoginQuery.h"
#include "AddUserQuery.h"
#include "WriteTagQuery.h"
#include "MarksHistory.h"
#include "ApplyChannelQuery.h"
#include "SubscribeChannelQuery.h"
#include "Channel.h"

class Client : public QObject
{
  Q_OBJECT
    private:
    int m_trackInterval;
    bool m_authentificated;
    bool m_trackingPermitted;

    QTimer * m_timer;

    WriteTagQuery * m_addNewMarkQuery;
    SubscribeChannelQuery *m_subscribeChannelQuery;
    ApplyChannelQuery *m_applyChannelQuery;
    MarksHistory * m_history;

    QString m_lastError;
    LoginQuery * m_loginQuery;
    AddUserQuery * m_addUserQuery;
    QSharedPointer<common::User> m_user;
    QNetworkConfigurationManager * m_netManager;
    QMap<QSharedPointer<Channel>, QString> m_channels;

    void pause(int msecs);

  public:
    explicit Client(QObject *parent = 0);
    // stub for authentification mechanizm

    bool isAuthentificated();
    QString getLastError();
    bool isOnline();
    void sendHistory();
    bool isTracking();
    void setHistoryLimit(int sec);
    void setPermission(bool permission);
    bool isTrackingPermitted();

  private slots:
    void onError(QString error);
    void onError(int err);
    void onAuthentificated();
    void onRegistered();
    void onMarkAdded();
    void track();
    signals:
    void error(QVariant error);
    void authentificated(QVariant);
    void authRequest();
  public slots:
    void auth(QString user, QString pass);
    void registration(QString user, QString pass);
    void subscribeChannel(QSharedPointer<Channel> m_channel);
    void onChannelSubscribed(QSharedPointer<Channel> m_channel);
    void startTrack();
    void stopTrack();
    // Network going down|up
    void onNetworkEvent(bool state);
    // on closed || on low battery
    void onGoOffEvent();
    // Send last added coordinate
    void sendLastCoordinate();
    // When history is full
    void onHistoryFull();

};
#endif                                  // CLIENT_H
