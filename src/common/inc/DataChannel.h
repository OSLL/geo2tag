#ifndef DATACHANNEL_H
#define DATACHANNEL_H

#include <QSharedPointer>
#include "Channel.h"
#include "DataMarks.h"

namespace common
{
    class DataChannel
    {
        QSharedPointer<Channel> m_channel;

        QSharedPointer<DataMarks> m_marks;

    public:
        DataChannel(const QSharedPointer<Channel>& channel, const QSharedPointer<DataMarks> marks);

        virtual ~DataChannel();
    };

};
#endif // DATACHANNEL_H
