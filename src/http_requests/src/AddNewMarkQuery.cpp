#include "AddNewMarkQuery.h"
#include "defines.h"
#include "AddNewMarkRequestJSON.h"
#include "AddNewMarkResponseJSON.h"

AddNewMarkQuery::AddNewMarkQuery(const QSharedPointer<DataMark> &tag, QObject *parent): DefaultQuery(parent), m_tag(tag)
{

}


QString AddNewMarkQuery::getUrl() const
{
   return APPLY_HTTP_URL;
}

QByteArray AddNewMarkQuery::getRequestBody() const
{
    AddNewMarkRequestJSON request;
    request.addTag(m_tag);
    return request.getJson();
}

void AddNewMarkQuery::processReply(QNetworkReply *reply)
{
    AddNewMarkResponseJSON response;
    response.parseJson(reply->readAll());
    if(response.getStatus() == "Ok")
    {
        Q_EMIT tagAdded();
    }
    else
    {
        Q_EMIT errorOccured(response.getStatusMessage());
    }
}

AddNewMarkQuery::~AddNewMarkQuery()
{

}

QSharedPointer<DataMark> AddNewMarkQuery::getTag()
{
    return m_tag;
}

const QSharedPointer<DataMark>& AddNewMarkQuery::getTag() const
{
    return m_tag;
}
