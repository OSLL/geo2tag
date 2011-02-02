#ifndef QUERYEXECUTOR_H
#define QUERYEXECUTOR_H

#include <QObject>
#include <QSqlDatabase>

#include "User.h"
#include "Channel.h"
#include "DataMarks.h"

class QueryExecutor : public QObject
{
    Q_OBJECT
    QSqlDatabase m_database;

    qlonglong nextKey(const QString& sequence) const;

public:

    QueryExecutor(const QSqlDatabase &db, QObject *parent = 0);

    bool isConnected();
    bool connect();

    qlonglong nextUserKey() const;
    qlonglong nextTagKey() const;
		const QString& generateNewToken(const QString login,const QString password) const;

    bool subscribeChannel(const QSharedPointer<User>& user,const QSharedPointer<Channel>& channel);
    QSharedPointer<DataMark> insertNewTag(const QSharedPointer<DataMark>&);
    QSharedPointer<User> insertNewUser(const QSharedPointer<User>&);

signals:

public slots:

};

#endif // QUERYEXECUTOR_H
