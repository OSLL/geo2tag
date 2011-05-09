#ifndef LOGINRESPONSEJSON_H
#define LOGINRESPONSEJSON_H

#include "JsonSerializer.h"

class LoginResponseJSON : public JsonSerializer
{
  Q_OBJECT;
  public:
    LoginResponseJSON(QObject *parent=0);

    QByteArray getJson() const;

    void parseJson(const QByteArray&);
};
// LOGINRESPONSEJSON_H
#endif
