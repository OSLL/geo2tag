#ifndef DATATHREAD_H
#define DATATHREAD_H

#include <QThread>
#include <QSettings>
#include "LightMap.h"

#include "LoginQuery.h"
#include "RSSFeedQuery.h"

class DataThread : public QThread
{
    Q_OBJECT

    QSettings m_settings;
    DataChannels m_marks; // here marks are stored
    QSharedPointer<User> m_user;
    LightMap* m_map;

    LoginQuery * m_loginQuery; // this field needs because query is asynchronous
    RSSFeedQuery * m_rssQuery; // this field needs because query is asynchronous

    bool m_isConnected;
    bool m_requestIsSended;

    void run();
        

protected slots:
    void onConnected();
    void onMarksGotten();
    void onGetFeed();
    void onError(QString);

public:
    DataThread(LightMap* map,QObject *parent = 0);

signals:
    void getFeed();
public slots:

};

#endif // DATATHREAD_H
