#ifndef ADDNEWMARKRESPONSEJSON_H
#define ADDNEWMARKRESPONSEJSON_H

#include "DefaultResponseJSON.h"

#include "JsonSerializer.h"

class AddNewMarkResponseJSON : public JsonSerializer
{
        Q_OBJECT;
public:
    AddNewMarkResponseJSON(QObject *parent=0);

    QByteArray getJson() const;

    void parseJson(const QByteArray&);

};
