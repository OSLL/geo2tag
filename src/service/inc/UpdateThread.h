#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H

#include <QtSql>
#include <QThread>
#include <QReadWriteLock>
#include <QMap>
#include "DataMarkInternal.h"
#include "UserInternal.h"
#include "ChannelInternal.h"

class UpdateThread: public QThread
{
    Q_OBJECT

    QSharedPointer<Channels>    m_channelsContainer;
    QSharedPointer<DataMarks>   m_tagsContainer;
    QSharedPointer<Users>       m_usersContainer;

    QSqlDatabase m_database;

    QReadWriteLock m_updateLock; //will be locked when containers is being updated

    void loadUsers(Users &);
    void loadTags(DataMarks &);
    void loadChannels(Channels &);
    void updateReflections(DataMarks&, Users&, Channels&);

    void run();

public:
    UpdateThread(
            const QSqlDatabase &db,
            const QSharedPointer<DataMarks>& tags,
            const QSharedPointer<Users>& users,
            const QSharedPointer<Channels>& channels,
            QObject *parent = 0);

    void lockWriting();

    void unlockWriting();

signals:

public slots:

};

#endif // UPDATETHREAD_H
