
#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QDebug>

#include "GetTimeSlotMarkResponseJSON.h"
#include "JsonUser.h"
#include "JsonTimeSlot.h"
#include "JsonDataMark.h"
#include "DataMarks.h"

GetTimeSlotMarkResponseJSON::GetTimeSlotMarkResponseJSON(QObject *parent) : JsonSerializer(parent)
{
}

QByteArray GetTimeSlotMarkResponseJSON::getJson() const
{
    QJson::Serializer serializer;
    QVariantMap obj;

    QSharedPointer<DataMark> mark = m_tagsContainer->at(0);

    obj.insert("timeSlot", mark->getTimeSlot()->getSlot());
    return serializer.serialize(obj);
}

void GetTimeSlotMarkResponseJSON::parseJson(const QByteArray &data)
{
    clearContainers();

    QJson::Parser parser;
    bool ok;

    QVariantMap result = parser.parse(data, &ok).toMap();
    if (!ok)
    {
        qFatal("An error occured during parsing json with channel list");
    }

    qulonglong slot = result["timeSlot"].toULongLong();
    QSharedPointer<TimeSlot>  timeSlot(new JsonTimeSlot(slot));
    QSharedPointer<DataMark> mark(new JsonDataMark(0,0,"unknown", "unknown", "unknown", QDateTime()));
    mark->setTimeSlot(timeSlot);
    m_tagsContainer->push_back(mark);
}

