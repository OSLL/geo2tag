#ifndef DATATHREAD_H
#define DATATHREAD_H

#include <QThread>
#include <QSettings>


#include "LoginQuery.h"
#include "RSSFeedQuery.h"

class DataThread : public QThread
{
    Q_OBJECT

    QSettings m_settings;

    LoginQuery * m_loginQuery; // this field needs because query is asynchronous
    RSSFeedQuery * m_rssQuery; // this field needs because query is asynchronous

    bool m_isConnected;

    void run();

protected slots:
    void onConnected();
    void onError(QString);

public:
    DataThread(QObject *parent = 0);

signals:

public slots:

};

#endif // DATATHREAD_H
