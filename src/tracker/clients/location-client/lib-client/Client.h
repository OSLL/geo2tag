#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QNetworkConfigurationManager>

#include "LoginQuery.h"


class Client : public QObject
{
    Q_OBJECT
private:
  bool m_authentificated;

  QString m_lastError;

  LoginQuery * m_loginQuery;

  QSharedPointer<common::User> m_user;
  QNetworkConfigurationManager * m_netManager;

  void pause(int msecs);

public:
    explicit Client(QObject *parent = 0);
    // stub for authentification mechanizm

    bool isAuthentificated();
    QString getLastError();
    bool isOnline();

private slots:
    void onError(QString error);
    void onError(int err);
    void onAuthentificated();
signals:
    void error(QVariant error);
    void authentificated(QVariant);
public slots:
    void auth(QString user, QString pass);

    
};

#endif // CLIENT_H
