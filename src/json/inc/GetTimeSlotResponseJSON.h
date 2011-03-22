#ifndef GETTIMESLOTRESPONSEJSON_H
#define GETTIMESLOTRESPONSEJSON_H

#include "JsonSerializer.h"

class GetTimeSlotResponseJSON : public JsonSerializer
{
	Q_OBJECT;
public:
    GetTimeSlotResponseJSON();

    QByteArray getJson() const;

    void parseJson(const QByteArray&);

};

#endif // GETTIMESLOTRESPONSEJSON_H
