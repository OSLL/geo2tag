#include "inc/EventsService.h"
#include <QDebug>
#include <QTimer>
#include "defines.h"
#include "Channel.h"
#include "DataChannel.h"
#include "JsonUser.h"
#include <QThread>

EventsService::EventsService(LocationManager *locationManager, QObject *parent) :
    QObject(parent),
    m_locationManager(locationManager),
    m_filterCircleQuery(0)
    //m_loadTagsQuery(0)
{
}

void EventsService::startService(QString name, QString password, QString authToken, QString serverUrl)
{
    qDebug() << "EventsService::startService " << this->thread()->currentThreadId();

    //if (m_loadTagsQuery != 0)
    //    m_loadTagsQuery->deleteLater();

    //m_loadTagsQuery = new LoadTagsQuery(this);
    //connect(m_loadTagsQuery, SIGNAL(tagsReceived()), this, SLOT(onTagsReceived()));
    //connect(m_loadTagsQuery, SIGNAL(errorOccured(QString)), this, SLOT(onError(QString)));

    if (m_filterCircleQuery != 0)
        m_filterCircleQuery->deleteLater();

    m_filterCircleQuery = new FilterCircleQuery(this);
    connect(m_filterCircleQuery, SIGNAL(tagsReceived()), this, SLOT(onTagsReceived()));
    connect(m_filterCircleQuery, SIGNAL(errorOccured(QString)), this, SLOT(onError(QString)));

    m_user = QSharedPointer<JsonUser>(new JsonUser(name, password, authToken));
    //m_loadTagsQuery->setUrl(serverUrl + FEED_HTTP_URL);
    m_filterCircleQuery->setUrl(serverUrl + FILTER_CIRCLE_HTTP_URL);

    requestEvents();
}

void EventsService::stopService()
{
}

void EventsService::requestEvents()
{
    qDebug() << "EventsService::requestEvents";
    QGeoPositionInfo info = m_locationManager->getInfo();
    if (info.isValid()) {
        //m_loadTagsQuery->setQuery(m_user, info.coordinate().latitude(),
        //                          info.coordinate().longitude(), EVENTS_RADIUS);

        QDateTime currentTime = QDateTime::currentDateTimeUtc();

        // just for sure that all newest events will be received
        QDateTime timeTo = currentTime.addYears(1);

        QDateTime timeFrom = currentTime.addSecs( - RELEVANT_PERIOD_IN_HOURS * 60 * 60);

        m_filterCircleQuery->setQuery(m_user, info.coordinate().latitude(),
                                      info.coordinate().longitude(), EVENTS_RADIUS,
                                      timeFrom, timeTo);
        qDebug() << "do events service request";
        //m_loadTagsQuery->doRequest();
        m_filterCircleQuery->doRequest();
    } else {
        qDebug() << "invalid geo info, waitin and trying again";
        QTimer::singleShot(5 * 1000, this, SLOT(requestEvents()));
    }
}

void EventsService::onTagsReceived()
{
    qDebug() << "EventsService::onTagsReceived";
    //DataChannels data = m_loadTagsQuery->getData();
    DataChannels data = m_filterCircleQuery->getData();
    QList<QSharedPointer<Channel> > channels = data.keys();
    Events events;
    bool found = false;
    for (int i = 0; i < channels.size(); ++i) {
        qDebug() << "channel: " << channels[i]->getName();
        if (channels[i]->getName() == "Events") {
            found = true;
            events = data.values(channels[i]);
            break;
        }
    }

    if (!NOT_RECEIVE_OWN_EVENTS) {
        QMutableListIterator<QSharedPointer<DataMark> > iter(events);
        while (iter.hasNext()) {
            QSharedPointer<DataMark> event = iter.next();
            qDebug() << event->getUser()->getLogin();
            if (event->getUser()->getLogin() == m_user->getLogin()) {
                iter.remove();
            }
        }
    }

    if (found) {
        // check them that new received
        emit eventsReceived(events);
    }

    QTimer::singleShot(5 * 1000, this, SLOT(requestEvents()));
}

void EventsService::onError(QString error)
{
    qDebug() << "EventsService::onErrorOccured error: " << error;
    emit errorOccured(error);
    QTimer::singleShot(5 * 1000, this, SLOT(requestEvents()));
}
