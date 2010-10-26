#include "RequestReceiver.h"

#include <QDataStream>

RequestReceiver::RequestReceiver(QTcpSocket *socket, QObject *parent) :
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
    m_socket = socket;
}

void RequestReceiver::sendStatus(struct Status status)
{
//    if (m_socket->open(QIODevice::ReadWrite))
//    {
        QDataStream out(m_socket);
        out << status.datetime;
        out << status.status;
        out << status.description;
//    }

//    m_socket->close();
}

void RequestReceiver::onSocketReadyRead()
{
  /*  if (!m_socket->open(QIODevice::ReadWrite))
    {
        return;
    }*/

    QDataStream in(m_socket);
    QString request;
    in >> request;
    qDebug() << "server recievs message " << request;
    if (request == "login")
    {
        QString name;
        QString password;
        in >> name >> password;
        emit login(name, password);
    }
    else if (request == "set_channel")
    {
        QString name;
        QString key;
        in >> name >> key;
        emit setChannel(name, key);
    }
    else if (request == "add channel")
    {
        QString name;
        QString key;
        in >> name >> key;
        emit addChannel(name, key);
    }
    else if (request == "start")
    {
        emit start();
    }
    else if (request == "stop")
    {
        emit stop();
    }
    else if (request == "status")
    {
        emit status();
    }

//    m_socket->close();
}
