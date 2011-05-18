#ifndef GENERATORDAEMON_H_
#define GENERATORDAEMON_H_

#include <QObject>
#include <QThread>
#include <QList>
#include <QPointF>
#include <QSharedPointer>
#include "LoginQuery.h"
#include "AddNewMarkQuery.h"
#include <QFile>
class GeneratorDaemon: public QThread
{
  Q_OBJECT;

  // User gotten after auth
  QSharedPointer<User> m_user;

  // Track channelname
  QString m_channelName;

  // List, where marks are stored
  QList<QPointF> m_marks;

  // Read all marks from file, specified in constructor, to m_marks
  void readMarks();

  // This function will run thread
  void run();

  // Query needed to recieve auth_token
  LoginQuery * m_loginQuery;

  // Query needed to send marks
  AddNewMarkQuery * m_tagQuery;

  bool m_isConnected;
  public:
    GeneratorDaemon();
    GeneratorDaemon(const QString& channelName);
    ~GeneratorDaemon();
  private slots:
    void onTagAdded();
    void onConnected();
    void onError(QString error);
};
#endif
