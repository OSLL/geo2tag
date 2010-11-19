#include <qjson/parser.h>
#include <qjson/serializer.h>

#include "LoginResponseJSON.h"
#include "JsonUser.h"

LoginResponseJSON::LoginResponseJSON()
{
}

QByteArray LoginResponseJSON::getJson() const
{
    QJson::Serializer serializer;
    QVariantMap obj;
    obj.insert("user", m_usersContainer->at(0)->getLogin());
    obj.insert("password", m_usersContainer->at(0)->getPassword());
    return serializer.serialize(obj);
}

void LoginResponseJSON::parseJson(const QByteArray &data)
{
    clearContainers();

    QJson::Parser parser;
    bool ok;

    QVariantMap result = parser.parse(data, &ok).toMap();
    if (!ok)
    {
        qFatal("An error occured during parsing json with channel list");
    }

    m_status = result["status"].toString();
    m_statusMessage = result["status_description"].toString();

    QString auth_token = result["auth_token"].toString();
    QSharedPointer<User> user(new JsonUser("unknown", "unknown", auth_token));
    m_usersContainer->push_back(user);
}
