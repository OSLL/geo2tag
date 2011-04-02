#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QDebug>

#include "SetTimeSlotResponseJSON.h"
#include "JsonUser.h"
#include "JsonChannel.h"
#include "JsonTimeSlot.h"
#include "Channel.h"

SetTimeSlotResponseJSON::SetTimeSlotResponseJSON()
{
}

QByteArray SetTimeSlotResponseJSON::getJson() const
{
    QJson::Serializer serializer;
    QVariantMap obj;

    obj.insert("status", m_status);
    obj.insert("status_description", m_statusMessage);
    return serializer.serialize(obj);
}

void SetTimeSlotResponseJSON::parseJson(const QByteArray &data)
{
    clearContainers();

    QJson::Parser parser;
    bool ok;

    QVariantMap result = parser.parse(data, &ok).toMap();
    if (!ok)
    {
        qFatal("An error occured during parsing json with channel list");
    }

    m_status = result["status"].toString();
    m_statusMessage = result["status_description"].toString();

}

