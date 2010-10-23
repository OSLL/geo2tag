#include "RequestSender.h"

#include <QDataStream>
#include "defines.h"

RequestSender::RequestSender(QTcpSocket *socket, QObject *parent) :
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
    if (m_socket->open(QIODevice::ReadWrite))
    {
        QDataStream out(m_socket);
        out << QString("login");
        out << name;
        out << password;
    }
    m_socket->close();
}

bool RequestSender::setChannel(QString name, QString key)
{
    if (m_socket->open(QIODevice::ReadWrite))
    {
        QDataStream out(m_socket);
        out << QString("set channel");
        out << name;
        out << key;
    }
    m_socket->close();
}

bool RequestSender::addChannel(QString name, QString key)
{
    if (m_socket->open(QIODevice::ReadWrite))
    {
        QDataStream out(m_socket);
        out << QString("add channel");
        out << name;
        out << key;
    }
    m_socket->close();
}

bool RequestSender::start()
{
    if (m_socket->open(QIODevice::ReadWrite))
    {
        QDataStream out(m_socket);
        out << QString("start");
    }
    m_socket->close();
}

bool RequestSender::stop()
{
    if (m_socket->open(QIODevice::ReadWrite))
    {
        QDataStream out(m_socket);
        out << QString("stop");
    }
    m_socket->close();
}

struct Status RequestSender::status()
{
    struct Status status;
    status.valid = false;

    if (m_socket->open(QIODevice::ReadWrite))
    {
        QDataStream out(m_socket);
        out << QString("status");
    }
    else
    {
        return status;
    }
    m_socket->close();

    QDateTime statusDatetime;
    QString statusStatus("");
    QString statusDescription("");
    QDataStream in(m_socket);
    m_socket->waitForReadyRead(TRACKER_MSECS_TIMEOUT);
    if (m_socket->open(QIODevice::ReadWrite))
    {
        in >> statusDatetime;
        in >> statusStatus;
        in >> statusDescription;
        status.valid = true;
    }
    status.datetime = statusDatetime;
    status.status = statusStatus;
    status.description = statusDescription;
    m_socket->close();
    return status;
}


