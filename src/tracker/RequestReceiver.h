#ifndef REQUESTRECEIVER_H
#define REQUESTRECEIVER_H

#include <QTcpSocket>

class RequestReceiver : public QObject
{
    Q_OBJECT

    QTcpSocket *m_socket;

public:
    RequestReceiver(QTcpSocket *socket, QObject *parent = 0);

    void setSocket(QTcpSocket *socket);
    QTcpSocket* getSocket();

    void sendStatus(QString status);

private slots:
    /* Reads data from socket and emits signals */
    void onSocketReadyRead();

signals:
    void login(QString name, QString password);
    void setChannel(QString name, QString key);
    void addChannel(QString name, QString key);
    void start();
    void stop();
    void status();
};

#endif // REQUESTRECEIVER_H
