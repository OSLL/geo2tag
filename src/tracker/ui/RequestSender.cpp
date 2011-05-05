#include "RequestSender.h"
#include <QDebug>
#include <QDataStream>
#include "defines.h"

RequestSender::RequestSender(QTcpSocket *socket, QObject *parent) :
QObject(parent),
m_socket(socket)
{
  qDebug() << "got " << socket << " socket";
  m_status.valid=true;
  m_status.status="Daemon is not connected";
  connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}


QTcpSocket* RequestSender::getSocket()
{
  return m_socket;
}


void RequestSender::setSocket(QTcpSocket *socket)
{
  qDebug() << "got " << socket << " socket";
  m_socket = socket;
  connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}


bool RequestSender::login(QString name, QString password)
{
  //    if (m_socket->open(QIODevice::ReadWrite))
  //    {
  QDataStream out(m_socket);
  out << QString("login");
  out << name;
  out << password;
  return true;
  //    }
  //    m_socket->close();
}


bool RequestSender::setChannel(QString name, QString key)
{
  //    if (m_socket->open(QIODevice::ReadWrite))
  //    {
  QDataStream out(m_socket);
  out << QString("set channel");
  out << name;
  out << key;
  return true;
  //    }
  //    m_socket->close();
}


bool RequestSender::addChannel(QString name, QString key)
{
  //    if (m_socket->open(QIODevice::ReadWrite))
  //    {
  QDataStream out(m_socket);
  out << QString("add channel");
  out << name;
  out << key;
  return true;
  //    }
  //    m_socket->close();
}


bool RequestSender::start()
{
  //    if (m_socket->open(QIODevice::ReadWrite))
  //    {
  QDataStream out(m_socket);
  out << QString("start");
  return true;
  //    }
  //    m_socket->close();
}


bool RequestSender::stop()
{
  //    if (m_socket->open(QIODevice::ReadWrite))
  //    {
  QDataStream out(m_socket);
  out << QString("stop");
  return true;
  //    }
  //    m_socket->close();
}


struct Status RequestSender::status()
{
  struct Status status;
  status.valid = false;

  /*    if (m_socket->open(QIODevice::ReadWrite))
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
      m_socket->close();*/
  return m_status;
}


void RequestSender::readyRead()
{
  QDataStream in(m_socket);
  QDateTime statusDatetime;
  QString statusStatus("");
  QString statusDescription("");
  struct Status status;
  status.valid = false;
  in >> statusDatetime;
  in >> statusStatus;
  in >> statusDescription;
  status.valid = true;
  status.datetime = statusDatetime;
  status.status = statusStatus;
  status.description = statusDescription;
  m_status=status;
}
