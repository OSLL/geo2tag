#ifndef LOGINREQUESTJSON_H
#define LOGINREQUESTJSON_H
#include "JsonSerializer.h"

class LoginRequestJSON: public JsonSerializer
{
  public:
    LoginRequestJSON(QObject *parent=0);

    QByteArray getJson() const;

    void parseJson(const QByteArray&);
    };
#endif                                                      // LOGINREQUESTJSON_H
