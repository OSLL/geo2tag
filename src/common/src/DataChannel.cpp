#include "DataChannel.h"
#include <QSharedData>

namespace common
{
    DataChannel::DataChannel(const QSharedPointer<Channel>& channel, const QSharedPointer<DataMarks> marks):
            m_channel(channel),
            m_marks(marks)
    {
    }

    DataChannel::~DataChannel()
    {
    }

}
