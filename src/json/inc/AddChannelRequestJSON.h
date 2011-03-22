#ifndef ADDCHANNELREQUESTJSON_H_
#define ADDCHANNELREQUESTJSON_H_

#include "JsonSerializer.h"

class AddChannelRequestJSON: public JsonSerializer
{
	Q_OBJECT;
public:
    AddChannelRequestJSON();

    QByteArray getJson() const;

    void parseJson(const QByteArray&);
};
#endif
