#ifndef DATACHANNEL_H
#define DATACHANNEL_H

#include <QSharedData>
#include <QSharedPointer>
#include "Channel.h"
#include "DataMarks.h"

namespace common
{
    class DataChannel : public QSharedData
    {
        QSharedPointer<Channel> m_channel;

        QSharedPointer<DataMarks> m_marks;

    public:
        DataChannel(const QSharedPointer<Channel>& channel, const QSharedPointer<DataMarks> marks);

        DataChannel(const DataChannel &);
        DataChannel &operator=(const DataChannel &);

        virtual ~DataChannel();
    };

};
#endif // DATACHANNEL_H
