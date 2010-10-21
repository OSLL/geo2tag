#include "RequestSender.h"

RequestSender::RequestSender(QTcpSocket *socket, QObject *parent = 0) :
        QObject(parent),
        m_socket(socket)
{
}

QTcpSocket* RequestSender::getSocket()
{
    return m_socket;
}

void RequestSender::setSocket(QTcpSocket *socket)
{
    m_socket = socket;
}

bool RequestSender::login(QString name, QString password)
{
    // TODO
}

bool RequestSender::setChannel(QString name, QString key)
{
    // TODO
}

bool RequestSender::addChannel(QString name, QString key)
{
    // TODO
}

bool RequestSender::start()
{
    // TODO
}

bool RequestSender::stop()
{

}

QString RequestSender::status()
{
    // TODO
}


