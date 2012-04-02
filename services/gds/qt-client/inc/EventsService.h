#ifndef EVENTSSERVICE_H
#define EVENTSSERVICE_H

#include <QObject>
#include <QSet>
#include <QList>
#include <QSharedPointer>
//#include "LoadTagsQuery.h"
#include "LocationManager.h"
#include "FilterCircleQuery.h"
#include "User.h"

typedef QList<QSharedPointer<DataMark> > Events;
Q_DECLARE_METATYPE(Events)

class EventsService : public QObject
{
    Q_OBJECT

    //LoadTagsQuery *m_loadTagsQuery;
    FilterCircleQuery *m_filterCircleQuery;
    QSharedPointer<common::User> m_user;

    LocationManager *m_locationManager;
    QSet<int> m_eventsIds;

public:
    explicit EventsService(LocationManager *locationManager, QObject *parent = 0);

public slots:
    void requestEvents();
    void startService(QString name, QString password, QString auth_token, QString serverUrl);
    void stopService();

signals:
    void eventsReceived(Events events);
    void errorOccured(QString error);

private slots:
    void onTagsReceived();
    void onError(QString error);

};

#endif // EVENTSSERVICE_H
