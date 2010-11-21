#ifndef DEFAULTRESPONSEJSON_H
#define DEFAULTRESPONSEJSON_H

#include "JsonSerializer.h"

class DefaultResponseJSON : public JsonSerializer
{
public:
    DefaultResponseJSON();

    QByteArray getJson() const;

    void parseJson(const QByteArray&);
};

#endif // DEFAULTRESPONSEJSON_H
