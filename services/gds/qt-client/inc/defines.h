#ifndef DEFINES_H
#define DEFINES_H

#define DEFAULT_USER_NAME       "Mark"
#define DEFAULT_USER_PASSWORD   "test"
#define DEFAULT_REMEMBER         true
#define DEFAULT_URL             "http://127.0.0.1/"

#define SUBSCRIBE_HTTP_URL        "service/subscribe"
#define UNSUBSCRIBE_HTTP_URL      QString(DEFAULT_URL)+QString("service/unsubscribe")
#define APPLY_HTTP_URL            QString(DEFAULT_URL)+QString("service/writeTag")
#define FEED_HTTP_URL             QString(DEFAULT_URL)+QString("service/loadTags")
#define SUBSCRIBED_LIST_HTTP_URL  QString(DEFAULT_URL)+QString("service/subscribed")
#define AVAILABLE_LIST_HTTP_URL   QString(DEFAULT_URL)+QString("service/channels")
#define LOGIN_HTTP_URL            "/service/login"
#define APPLY_CHANNEL_HTTP_URL    QString(DEFAULT_URL)+QString("service/addChannel")
#define ADD_USER_HTTP_URL         QString(DEFAULT_URL)+QString("service/addUser")

#endif
