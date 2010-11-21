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

#include "LoginQuery.h"
#include "AddNewMarkQuery.h"

class TrackerDaemon : public QThread
{
    Q_OBJECT;

    QSettings m_settings;

    LoginQuery * m_loginQuery; // this field needs because query is asynchronous
    AddNewMarkQuery * m_tagQuery; // this field needs because query is asynchronous

    bool m_exitFlag;
    bool m_isConnected;

    void run();
private slots:

    void onConnected(); //calls when login/pass were accepted
    void onTagAdded();
    void onError(QString);

public:
    TrackerDaemon();

    void stopTracking();
    void startTracking();

    ~TrackerDaemon();

public slots:


};

#endif // TRACKERDAEMON_H
