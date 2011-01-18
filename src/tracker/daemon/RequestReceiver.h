#ifndef REQUESTRECEIVER_H
#define REQUESTRECEIVER_H

#include <QTcpSocket>
#include "Status.h"

class RequestReceiver : public QObject
{
    Q_OBJECT

    QTcpSocket *m_socket;

public:
    RequestReceiver(QTcpSocket *socket, QObject *parent = 0);

    void setSocket(QTcpSocket *socket);
    QTcpSocket* getSocket();

    /* daemon can sent its status to gui through socket
       if it was requested */
    void sendStatus(struct Status status);

private slots:
    /* Reads data from socket and emits signals */
    void onSocketReadyRead();

signals:
    /* is emittend whent gui have sent a login request */
    void login(QString name, QString password);

    /* is emittend whent gui have sent a request to set a channel */
    void setChannel(QString name, QString key);

    /* is emittend whent gui have sent a request to add a channel */
    void addChannel(QString name, QString key);

     /* is emittend whent gui have sent a request to start sending
        of marks with position */
    void start();

    /* is emittend whent gui have sent a request to stop sending
       of marks with position */
    void stop();

    /* is emitted when gui have requested a status */
    void status();
};

#endif // REQUESTRECEIVER_H
