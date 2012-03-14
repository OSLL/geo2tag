#ifndef ADDUSERQUERY_H
#define ADDUSERQUERY_H
#include <QObject>
#include <QString>
#include "DefaultQuery.h"
#include "User.h"


class AddUserQuery: public DefaultQuery
{
  Q_OBJECT

    QString m_login;
  QString m_password;

  //!< full information about user
  QSharedPointer<common::User> m_user;

  virtual QString getUrl() const;
  virtual QByteArray getRequestBody() const;

  private Q_SLOTS:

    virtual void processReply(QNetworkReply *reply);

  public:

    AddUserQuery(const QString& login, const QString& password, QObject *parent = 0);

    AddUserQuery(QObject *parent = 0);

    void setQuery(const QString& login, const QString& password);

    QSharedPointer<common::User> getUser() const;

    ~AddUserQuery();

    Q_SIGNALS:

    void connected();


};
#endif // ADDUSERQUERY_H