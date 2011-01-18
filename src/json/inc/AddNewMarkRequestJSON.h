#ifndef ADDNEWMARKREQUESTJSON_H
#define ADDNEWMARKREQUESTJSON_H

#include "JsonSerializer.h"

class AddNewMarkRequestJSON : public JsonSerializer
{
public:

    AddNewMarkRequestJSON();

    virtual QByteArray getJson() const;

    virtual void parseJson(const QByteArray&);
};

#endif // ADDNEWMARKREQUESTJSON_H
