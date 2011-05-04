#ifndef ADDNEWMARKREQUESTJSON_H
#define ADDNEWMARKREQUESTJSON_H

#include "JsonSerializer.h"

class AddNewMarkRequestJSON : public JsonSerializer
{
  public:

    AddNewMarkRequestJSON(QObject *parent=0);

    virtual QByteArray getJson() const;

    virtual void parseJson(const QByteArray&);
};
// ADDNEWMARKREQUESTJSON_H
#endif
