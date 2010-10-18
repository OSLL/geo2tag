#include "GetStatus.h"
#include <QDataStream>

namespace TrackerRequests
{

    GetStatus::GetStatus(QTcpSocket *socket)
        : BaseRequest(socket)
    {
        this->setRequest("status");
    }

    int GetStatus::doRequest()
    {
        QDataStream out(m_socket);
        out << m_request;
        m_socket->waitForReadyRead(TRACKER_MSECS_TIMEOUT);
        QDataStream in(m_socket);
        in >> m_status;
        if (m_status == "error")
        {
            in >> m_errorDescription;
        }
    }

    int GetStatus::doResponse()
    {
        QDataStream out(m_socket);
        out << m_status;
        if (m_status == "error")
        {
            out << m_errorDescription;
        }
    }

    void GetStatus::setStatus(QString status)
    {
        m_status = status;
    }

    void GetStatus::setErrorDescription(QString errorDescription)
    {
        m_errorDescription = errorDescription;
    }

} // namespace TrackerRequests
