#ifndef BASEREQUEST_H
#define BASEREQUEST_H

#define TRACKER_MSECS_TIMEOUT 500

#include <QString>
#include <QDataStream>
#include <QTcpSocket>

namespace TrackerRequests
{

class BaseRequest
{
    QTcpSocket *m_socket;
    QString m_request;

public:
    BaseRequest(QTcpSocket *socket);

    void setRequest(QString request);
    QString getRequest();

    virtual int doRequest() = 0;
    virtual int doResponse() = 0;

};

} // namespace TrackerRequests

#endif // BASEREQUEST_H
