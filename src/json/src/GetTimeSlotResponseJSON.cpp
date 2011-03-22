#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QDebug>

#include "GetTimeSlotResponseJSON.h"
#include "JsonUser.h"
#include "JsonChannel.h"
#include "JsonTimeSlot.h"
#include "Channel.h"

GetTimeSlotResponseJSON::GetTimeSlotResponseJSON()
{
}

QByteArray GetTimeSlotResponseJSON::getJson() const
{
    QJson::Serializer serializer;
    QVariantMap obj;

    QSharedPointer<Channel> channel = m_channelsContainer->at(0);

    obj.insert("timeSlot", channel->getTimeSlot()->getId());
    return serializer.serialize(obj);
}

void GetTimeSlotResponseJSON::parseJson(const QByteArray &data)
{
    clearContainers();

    QJson::Parser parser;
    bool ok;

    QVariantMap result = parser.parse(data, &ok).toMap();
    if (!ok)
    {
        qFatal("An error occured during parsing json with channel list");
    }

    QString slot = result["timeSlot"].toString();
    QSharedPointer<TimeSlot>  timeSlot(new JsonTimeSlot(slot));
    QSharedPointer<Channel> channel(new JsonChannel("unknown", "unknown"));
    channel->setTimeSlot(timeSlot);
    m_channelsContainer->push_back(channel);
}

