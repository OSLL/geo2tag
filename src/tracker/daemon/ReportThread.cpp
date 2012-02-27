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

#include <QEventLoop>
#include <QStringList>
#include <QDebug>
#include "ReportThread.h"

ControlThread::ControlThread(QTcpSocket *socket, TrackerDaemon *control, QObject *parent):
QObject(parent), m_client(socket), m_device(new QTextStream(socket)),
m_daemon(qobject_cast<TrackerDaemon*>(control))
{
  m_processors.insert("start", &ControlThread::processStartQuery);
  m_processors.insert("stop", &ControlThread::processStopQuery);
  m_processors.insert("logs", &ControlThread::processLogsQuery);
  m_processors.insert("status", &ControlThread::processStatusQuery);
  m_processors.insert("reload", &ControlThread::processReloadQuery);
  connect(m_client,SIGNAL(readyRead()), SLOT(run()));
}


void ControlThread::run()
{
  QString query = m_device->readAll();
  qDebug() << "ControlThread running, query =" << query;
  QStringList commands=query.split(" ",QString::SkipEmptyParts);
  for (int i=0;i<commands.size();i++)
  {
    if(!commands.at(i).isEmpty() && m_processors.contains(commands.at(i)))
    {
      ProcessMethod method = m_processors.value(commands.at(i));
      (*this.*method)();
    }
  }
}


void ControlThread::processStartQuery()
{
  m_daemon->startTracking();
  (*m_device) << "lastCoords_" << m_daemon->getLastCoords().x() << "," << m_daemon->getLastCoords().y()<< " ";
  //(*m_device) << "started ";//"<status>started</status>";
  m_device->flush();
}


void ControlThread::processStopQuery()
{
  m_daemon->stopTracking();
  (*m_device) << "stoped ";             //"<status>stoped</status>";
  m_device->flush();
}


void ControlThread::processStatusQuery()
{
  if(m_daemon->isTracking())
    (*m_device) << "lastCoords_" << m_daemon->getLastCoords().x() << "," << m_daemon->getLastCoords().y()<<" ";
  //(*m_device) << "started ";//"<status>started</status>";
  else
    (*m_device) << "stoped ";           //"<status>stoped</status>";
  m_device->flush();
}


void ControlThread::processReloadQuery()
{
  m_daemon->reloadSettings();
}


void ControlThread::processLogsQuery()
{
  QStringList list = m_daemon->getLog();
  (*m_device) << "<log>";
  for(int i=0;i<list.size();i++)
    (*m_device) << list.at(i);
  (*m_device) << "</log>";
  m_device->flush();
}
