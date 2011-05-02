#ifndef GETTIMESLOTREQUESTJSON_H
#define GETTIMESLOTREQUESTJSON_H

#include "JsonSerializer.h"

class GetTimeSlotRequestJSON : public JsonSerializer
{
  Q_OBJECT;
  public:
    GetTimeSlotRequestJSON(QObject *parent=0);

    QByteArray getJson() const;

    void parseJson(const QByteArray&);
    };
#endif                                                      // GETTIMESLOTREQUESTJSON_H
