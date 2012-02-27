/*
 * Copyright 2010-2011  OSLL osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
/*!
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

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
