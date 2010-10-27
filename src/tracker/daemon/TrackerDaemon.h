#ifndef TRACKERDAEMON_H
#define TRACKERDAEMON_H

#include <QObject>
#include <QDateTime>
#include <QPointer>
#include <QTimerEvent>
#include "tracker.h"
#include <QTextStream>
#include <QFile>
#include "GpsInfo.h"

#include "ApplyMarkQuery.h"
#include "LoginQuery.h"

class TrackerDaemon : public QObject
{
    Q_OBJECT;
    QTextStream *m_logOut;
    QFile *m_log;
    Settings   m_settings;
    QDateTime  m_lastAttempt;

    QString m_status;
    QString m_statusDescription;
    int m_timerID;

    GUI::ApplyMarkQuery m_applyMarkQuery;
    GUI::LoginQuery m_loginQuery;

    void start();// start sending marks

    void login(QString login,QString password); // get auth tocken

    void initSettings();
    bool setMark();

public:
    TrackerDaemon();

    void timerEvent(QTimerEvent *te);

public slots:

    void stop();// stop sending marks


    // TODO refactor this method for writing into log file in /var/
    void setStatus(QString channel,QString channelKey);


    void readSettings();
    void setupBearer();

    void onLoginResponse(QString status,QString auth_token,QString status_description);
    void onApplyMarkResponse(QString,QString);


};

#endif // TRACKERDAEMON_H
