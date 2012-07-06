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
#include <QDebug>
#include "DataThread.h"
#include <QList>

DataThread::DataThread(MapScene * map,QObject * parent) :
QThread(parent),
m_settings(QSettings::SystemScope,"osll","observer"),
m_map(map),
m_isConnected(false),
m_requestIsSent(false)

{
  connect(this,SIGNAL(getFeed()),this,SLOT(onGetFeed()));
}


void DataThread::run()
{
  qDebug() << "thread started";
  QEventLoop eventLoop;
  QString login = m_settings.value("user").toString();
  QString password = m_settings.value("password").toString();
  if(login.isEmpty())
    login = "Mark";
  if(password.isEmpty())
    password = "test";

  m_loginQuery = new LoginQuery(login, password, parent());
  connect(m_loginQuery, SIGNAL(connected()), SLOT(onConnected()));
  connect(m_loginQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
  m_loginQuery->doRequest();
  for(;;)
  {
    if(m_isConnected)
    {
      qDebug() << "connected: auth_token=" << m_loginQuery->getSession()->getUser()->getToken();
      emit(getFeed());
      //ToDo query RSS

    }
    else
    {
      m_loginQuery->doRequest();
    }

    eventLoop.processEvents(QEventLoop::ExcludeUserInputEvents, 1000);
    QThread::msleep(5000);
  }
}


void DataThread::onConnected()
{
  m_isConnected = true;
  QSharedPointer<Session> session = m_loginQuery->getSession();
  m_user=session->getUser();
  m_rssQuery=new LoadTagsQuery(session,60.,30.,3000000.,parent());
  qDebug() << "rssFeedQuery setted" << m_rssQuery;
  connect(m_rssQuery, SIGNAL(tagsReceived()), this,SLOT(onMarksGotten()));
  connect(m_rssQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
}


void DataThread::onMarksGotten()
{
  m_marks=m_rssQuery->getData();
  m_map->setMarks(m_marks);
  QList<QSharedPointer<DataMark> > marks = m_marks.values();
  qDebug() << "RssFeed gotten!!!!! " <<  m_marks.count() << "marks received";
  m_requestIsSent=false;
  //and now redraw canvas
}


void DataThread::onGetFeed()
{
  qDebug() << "onGetFeed";
  if (!m_requestIsSent)
  {
    qDebug() << " m_rssQuery->doRequest()";
    m_rssQuery->doRequest();
    m_requestIsSent=true;
  }
}


void DataThread::onError(QString message)
{
  qDebug() << "Error occured!" << message ;
  m_requestIsSent = false;              // If request failed we need to try again
  m_isConnected = false;
}
