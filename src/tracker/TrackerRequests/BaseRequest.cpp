#include "BaseRequest.h"

namespace TrackerRequests
{

    BaseRequest::BaseRequest(QTcpSocket *socket)
    {
        m_socket = socket;
    }

    void BaseRequest::setRequest(QString request)
    {
        m_request = request;
    }

    QString BaseRequest::getRequest()
    {
        return m_request;
    }

} // namespace TrckerRequest
