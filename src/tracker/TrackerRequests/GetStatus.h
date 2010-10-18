#ifndef GETSTATUS_H
#define GETSTATUS_H

#include "BaseRequest.h"

namespace TrackerRequests
{

    class GetStatus : public BaseRequest
    {
        QString m_status;
        QString m_errorDescription;
    public:
        GetStatus(QTcpSocket *socket);
        virtual int doRequest();
        virtual int doResponse();

        void setStatus(QString status);
        void setErrorDescription(QString errorDescription);

        QString getStatus();
        QString getErrorDescription();
    };

} // namespace TrackerRequests

#endif // GETSTATUS_H
