#ifndef ADDCHANNELREQUESTJSON_H_
#define ADDCHANNELREQUESTJSON_H_

#include "JsonSerializer.h"

class AddChannelRequestJSON: public JsonSerializer
{
public:
    AddChannelRequestJSON();

    QByteArray getJson() const;

    void parseJson(const QByteArray&);
};
#endif
