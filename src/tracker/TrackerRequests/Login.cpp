#include "Login.h"
#include <QDataStream>

namespace TrackerRequests
{

    Login::Login(QTcpSocket *socket)
        : BaseRequest(socket)
    {
        this->setRequest("login");
    }

    int Login::doRequest()
    {
        QDataStream out(m_socket);
        out << m_request;
        m_socket->waitForReadyRead(TRACKER_MSECS_TIMEOUT);
        QDataStream in(m_socket);
        in >> m_status;
        if (m_status == "error")
        {
            in >> m_errorDescription;
            if (m_errorDescription == "network error")
            {
                in >> m_networkErrorDescription;
            }
        }
    }

    int Login::doResponse()
    {
        QDataStream out(m_socket);
        out << m_status;
        if (m_status == "error")
        {
            out << m_errorDescription;
            if (m_errorDescription == "network error")
            {
                out << m_networkErrorDescription;
            }
        }
    }

    void Login::setRequest(QString login, QString password)
    {
        m_login = login;
        m_password = password;
    }

    void Login::setResponse(QString status, QString errorDescription,
                            QString networkErrorDescription)
    {
        m_status = status;
        m_errorDescription = errorDescription;
        m_networkErrorDescription = networkErrorDescription;
    }

    QString Login::getLogin()
    {
        return m_login;
    }

    QString Login::getPassword()
    {
        return m_password;
    }

    QString Login::getStatus()
    {
        return status;
    }

    QString Login::getErrorDescription()
    {
        return m_errorDescription;
    }

    QString Login::getNetworkErrorDescription()
    {
        return m_networkErrorDescription;
    }

} // namespace TrackerRequests
