#ifndef LOGIN_H
#define LOGIN_H

#include "BaseRequest.h"

namespace TrackerRequests
{

    class Login : public BaseRequest
    {
        QString m_login;
        QString m_password;
        QString m_status;
        QString m_errorDescription;
        QString m_networkErrorDescription;

    public:
        Login(QTcpSocket *socket);
        Login(QTcpSocket *socket);

        virtual int doRequest();
        virtual int doResponse();

        void setRequest(QString login, QString password);
        void setResponse(QString status, QString errorDescription,
                         QString networkErrorDescription);

        QString getLogin();
        QString getPassword();
        QString getStatus();
        QString getErrorDescription();
        QString getNetworkErrorDescription();
    };

} // namespace TrackerRequests


#endif // LOGIN_H
