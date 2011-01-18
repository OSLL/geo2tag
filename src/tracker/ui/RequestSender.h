#ifndef REQUESTSENDER_H
#define REQUESTSENDER_H

#include <QTcpSocket>
#include "Status.h"

class RequestSender : public QObject
{
    Q_OBJECT
    Status m_status;
    QTcpSocket *m_socket;

public:

    RequestSender(QTcpSocket *socket, QObject *parent = 0);

    QTcpSocket* getSocket();
    void setSocket(QTcpSocket *socket);

    /* if daemon is not loged in then gui have to
       call this method with name/password */
    bool login(QString name, QString password);

    /* send channel to send marks with position in */
    bool setChannel(QString name, QString key);

    /* add channel if it is requred */
    bool addChannel(QString name, QString key);

    /* start sending of marks with position */
    bool start();

    /* stop sending if marks */
    bool stop();

    /* get status of the daemon */
    struct Status status();

private slots:
    void readyRead();

};

#endif // REQUESTSENDER_H
