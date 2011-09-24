#ifndef DATATHREAD_H
#define DATATHREAD_H

#include <QThread>
#include <QSettings>
#include "MapScene.h"

#include "LoginQuery.h"
#include "RSSFeedQuery.h"

class DataThread : public QThread
{
  Q_OBJECT

    QSettings m_settings;
  // here marks are stored
  DataChannels m_marks;
  QSharedPointer<common::User> m_user;
  MapScene * m_map;

  // this field needs because query is asynchronous
  LoginQuery * m_loginQuery;
  // this field needs because query is asynchronous
  RSSFeedQuery * m_rssQuery;

  bool m_isConnected;
  bool m_requestIsSent;

  void run();

  protected slots:
    void onConnected();
    void onMarksGotten();
    void onGetFeed();
    void onError(QString);

  public:
    DataThread(MapScene * map, QObject *parent = 0);

    signals:
    void getFeed();
  public slots:

};
// DATATHREAD_H
#endif
