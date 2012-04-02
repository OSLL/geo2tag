#ifndef DEFINES_H
#define DEFINES_H

#define DEFAULT_USER_NAME           "Mark"
#define DEFAULT_USER_PASSWORD       "test"
#define DEFAULT_REMEMBER            true
#define DEFAULT_TOKEN               "fake token"
#define DEFAULT_URL                 "http://127.0.0.1/"
#define DEFAULT_TRACKING_PERIOD     10
#define DEFAULT_LOCATION_TIMEOUT    10
#define EVENTS_CHANNEL              "Events"
#define EVENTS_RADIUS               30
#define RELEVANT_PERIOD_IN_HOURS    240

#define SUBSCRIBE_HTTP_URL        "/service/subscribe"
#define UNSUBSCRIBE_HTTP_URL      QString(DEFAULT_URL)+QString("service/unsubscribe")
#define APPLY_HTTP_URL            "/service/writeTag"
#define FEED_HTTP_URL             "/service/loadTags"
#define SUBSCRIBED_LIST_HTTP_URL  QString(DEFAULT_URL)+QString("service/subscribed")
#define AVAILABLE_LIST_HTTP_URL   QString(DEFAULT_URL)+QString("service/channels")
#define LOGIN_HTTP_URL            "/service/login"
#define ADD_CHANNEL_HTTP_URL    "/service/addChannel"
#define ADD_USER_HTTP_URL         "/service/addUser"
#define REGISTER_USER_HTTP_URL    "/service/addUser"
#define FILTER_CIRCLE_HTTP_URL    "/service/filterCircle"

#include <QString>
QString getErrnoDescription(int errno);

#endif
