#ifndef DATACHANNEL_H
#define DATACHANNEL_H

#include <QMultiHash>
#include "Channel.h"
#include "DataMarks.h"

typedef QMultiMap<QSharedPointer<Channel>, QSharedPointer<DataMark> > DataChannels;

#endif // DATACHANNEL_H
