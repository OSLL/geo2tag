#include "DataChannel.h"
#include <QSharedData>

namespace common
{
    DataChannel::DataChannel(const QSharedPointer<Channel>& channel, const QSharedPointer<DataMarks> marks):
            m_channel(channel),
            m_marks(marks)
    {
    }

    DataChannel::DataChannel(const DataChannel &rhs):
            QSharedData(rhs),
            m_channel(rhs.m_channel),
            m_marks(rhs.m_marks)
    {
    }

    DataChannel &DataChannel::operator=(const DataChannel &rhs)
    {
        if (this != &rhs)
        {
            m_channel.operator=(rhs.m_channel);
            m_marks.operator =(rhs.m_marks);
        }
        return *this;
    }

    DataChannel::~DataChannel()
    {
    }

}
