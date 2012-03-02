#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QNetworkConfigurationManager>
#include <QTimer>

#include "LoginQuery.h"
#include "WriteTagQuery.h"
#include "MarksHistory.h"
//#include "SubscribeChannelQuery.h"

class Client : public QObject
{
  Q_OBJECT
    private:
    int m_trackInterval;
    bool m_authentificated;

    QTimer * m_timer;

    WriteTagQuery * m_addNewMarkQuery;
    // SubscribeChannelQuery *m_subscribeChannelQuery;
    MarksHistory * m_history;

    QString m_lastError;
    LoginQuery * m_loginQuery;
    QSharedPointer<common::User> m_user;
    QNetworkConfigurationManager * m_netManager;

    void pause(int msecs);

  public:
    explicit Client(QObject *parent = 0);
    // stub for authentification mechanizm

    bool isAuthentificated();
    QString getLastError();
    bool isOnline();
    void sendHistory();
    bool isTracking();

  private slots:
    void onError(QString error);
    void onError(int err);
    void onAuthentificated();
    void onMarkAdded();
    void track();
    signals:
    void error(QVariant error);
    void authentificated(QVariant);
    void authRequest();
  public slots:
    void auth(QString user, QString pass);
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
