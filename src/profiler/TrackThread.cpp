/*
 * Copyright 2011  Mark Zaslavskiy  mark.zaslavskiy@gmail.com
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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

/*! ---------------------------------------------------------------
 *
 * \file TrackThread.cpp
 * \brief TrackThread implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "TrackThread.h"
#include <QDebug>
#include "ErrnoTypes.h"
#include <QEventLoop>
#include <QDateTime>

int TrackThread::m_counter=0;
int TrackThread::m_number_of_requests=0;

TrackThread::TrackThread(const QSharedPointer<DataMark> &tag)
{
  m_track = new WriteTagQuery(tag);
  connect(this,SIGNAL(doRequest()),this, SLOT(sendRequest()));
  connect(m_track,SIGNAL(tagAdded()),this, SLOT(responseRecieved()));
  connect(m_track,SIGNAL(errorOccured(int)),this, SLOT(responseRecieved()));
//  m_track->moveToThread(this);
}

TrackThread::~TrackThread()
{
  delete m_track;
}

void TrackThread::run()
{
  
 emit doRequest();
 exec();
}

int TrackThread::getCounter() 
{
  return m_counter;
}

void TrackThread::incCounter()
{
  m_counter++;
}


void TrackThread::sendRequest()
{

  m_sendTime = QDateTime::currentDateTime();
  m_track->doRequest();
}

void TrackThread::responseRecieved()
{
  incCounter();
  qDebug() << getCounter() << " " << m_sendTime.msecsTo(QDateTime::currentDateTime()) << " " << m_track->getErrno();
  if (m_counter == m_number_of_requests ) exit();
  emit doRequest();
}
