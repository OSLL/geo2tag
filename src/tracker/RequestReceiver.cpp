#include "RequestReceiver.h"

RequestReceiver::RequestReceiver(QTcpSocket *socket, QObject *parent = 0) :
        QObject(parent),
        m_socket(socket)
{
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

QTcpSocket* RequestReceiver::getSocket()
{
    return m_socket;
}

void RequestReceiver::setSocket(QTcpSocket *socket)
{
    m_soket = socket;
}

void RequestReceiver::sendStatus(QString status)
{
    // TODO
}

void RequestReceiver::onSocketReadyRead()
{
    // TODO
}
