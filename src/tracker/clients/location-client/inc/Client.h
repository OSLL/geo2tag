#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QNetworkConfigurationManager>
#include <QTimer>
#include <QMap>

#include "LoginQuery.h"
#include "RegisterUserQuery.h"
#include "WriteTagQuery.h"
#include "MarksHistory.h"
#include "ApplyChannelQuery.h"
#include "SubscribeChannelQuery.h"
#include "Channel.h"
#include "ContactModel.h"
#include "SubscribedChannelsQuery.h"
#include "LoadTagsQuery.h"
#include "UnsubscribeChannelQuery.h"

class Client : public QObject
{
  Q_OBJECT
    private:
    int m_trackInterval;
    bool m_authentificated;
    bool m_trackingPermitted;
    bool m_isHavingOwnChannel;

    QTimer * m_timer;
    QTimer * m_additionalTimer;

    WriteTagQuery * m_addNewMarkQuery;
    SubscribeChannelQuery *m_subscribeChannelQuery;
    ApplyChannelQuery *m_applyChannelQuery;
    MarksHistory * m_history;

    QString m_lastError;
    LoginQuery * m_loginQuery;
    RegisterUserQuery * m_RegisterUserQuery;
    QSharedPointer<common::User> m_user;
    QNetworkConfigurationManager * m_netManager;
    QMap<QSharedPointer<Channel>, QString> m_channels;
    ContactModel * m_contactModel;
    SubscribedChannelsQuery *m_subscibedChannelsQuery;
    LoadTagsQuery *m_loadTagsQuery;
    UnsubscribeChannelQuery *m_unsubscribeChannelQuery;

    void pause(int msecs);

  public:
    explicit Client(ContactModel* contactModel, QObject *parent = 0);
    // stub for authentification mechanizm

    bool isAuthentificated();
    QString getLastError();
    bool isOnline();
    void sendHistory();
    bool isTracking();
    void setHistoryLimit(int sec);
    void setPermission(bool permission);
    bool isTrackingPermitted();

    void unSubscribeChannelRequest(const QString &channelName);
    void logout();

  private slots:
    void onError(QString error);
    void onError(int err);
    void onAuthentificated();
    void onRegistered();
    void onMarkAdded();
    void track();
    void process(QNetworkReply*);
    void subscribeToOwnChannel();
    signals:
    void error(QVariant error);
    void authentificated(QVariant);
    void authRequest();
    void registrationRequestConfirm();
  public slots:
    void auth(QString user, QString pass);
    void registration(const QString &email,const QString& user, const QString& pass);
    void subscribeChannel(const QString &channel/*QSharedPointer<Channel> m_channel*/);
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

    // void constructContactModel();
    void getTagsRequest();
    void onGetTags();

    void onChannelUnsubscribed();

};
#endif                                  // CLIENT_H
