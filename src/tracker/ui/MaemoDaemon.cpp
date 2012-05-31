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

#include "MaemoDaemon.h"
#include <QStringList>
#include <QDebug>
MaemoDaemon::MaemoDaemon():
m_daemon(new QTcpSocket()),
m_device(new QTextStream(m_daemon)),
m_started(false)
{
  connect(m_daemon,SIGNAL(readyRead()), SLOT(readData()));
  m_daemon->connectToHost("127.0.0.1", 31234);
}


void MaemoDaemon::readData()
{

  QString message = m_device->readAll();

  // split received message for many parts, process last and clean
  QStringList commands=message.split(" ",QString::SkipEmptyParts);
  qDebug() << "received from trackerDaemon " << message;
  if (commands.last().indexOf("lastCoords_")!=-1)
  {
    QString buf=commands.last();
    QStringList lastCoord = buf.right(buf.size()-QString("lastCoords_").size()).split(",",QString::SkipEmptyParts);

    qDebug() << "Received coordinates" << lastCoord.at(0) << " "<< lastCoord.at(1);
    m_lastCoordinates.setX(lastCoord.at(0).toDouble());
    m_lastCoordinates.setY(lastCoord.at(1).toDouble());
    m_started = true;
  }
  if (commands.last()=="stoped")
  {
    m_started = false;
  }
}


void MaemoDaemon::start()
{
  (*m_device) << "start ";
}


void MaemoDaemon::stop()
{
  (*m_device) << "stop ";
}


void MaemoDaemon::reload()
{
  (*m_device) << "reload ";
}


void MaemoDaemon::restart()
{
  system("pkill  \'wikigpsTracker$\'");
  system("/usr/bin/wikigpsTracker > /dev/null 2>&1 &");
}


QPointF MaemoDaemon::getLastCoordinates() const
{
  return m_lastCoordinates;
}


bool MaemoDaemon::isConnected() const
{
  return (m_daemon->state() >= QAbstractSocket::ConnectedState);
}


bool MaemoDaemon::isStarted() const
{
  return m_started;
}
