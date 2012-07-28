/*
 * Copyright 2012  Mark Zaslavskiy  mark.zaslavskiy@gmail.com
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
 * \file ProfilerThread.cpp
 * \brief ProfilerThread implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "ProfilerThread.h"
#include "ErrnoTypes.h"
#include <QDebug>
#include <QEventLoop>
#include "defines.h"

int ProfilerThread::m_counter=0;
int ProfilerThread::m_number_of_requests=0;
double ProfilerThread::m_requests_per_second=0.;

ProfilerThread::ProfilerThread()
{
  m_sendTime = QDateTime::currentDateTime();
}


void ProfilerThread::setConnections()
{
  connect(this,SIGNAL(doRequest()),this, SLOT(sendRequest()));
  connect(m_query,SIGNAL(errorOccured(int)),this, SLOT(responseReceived()));
  connect(m_query,SIGNAL(success()),this, SLOT(responseReceived()));
}


ProfilerThread::~ProfilerThread()
{
  delete m_query;
}


void ProfilerThread::run()
{
  emit doRequest();
  exec();
}


int ProfilerThread::getCounter()
{
  return m_counter;
}


void ProfilerThread::incCounter()
{
  m_counter++;
}


void ProfilerThread::sendRequest()
{

  srand(time(NULL));
  m_requests_per_second = 1000./((double)m_sendTime.msecsTo(QDateTime::currentDateTime()));
  m_sendTime = QDateTime::currentDateTime();
  m_query->doRequest();
}


void ProfilerThread::responseReceived()
{
  if (m_query->getErrno() == SUCCESS) incCounter();
  qDebug() << getCounter() << m_sendTime.msecsTo(QDateTime::currentDateTime()) <<  (int)(m_query->getErrno()!=SUCCESS) << m_requests_per_second;
  //QString a=QString("echo \"%1 %2 %3 %4 `ps -e -o rss,comm | grep fcgi | grep -o \\\"^[0-9 ]*\\\"`\"").arg(getCounter()).arg(m_sendTime.msecsTo(QDateTime::currentDateTime())).arg((int)(m_query->getErrno()!=SUCCESS)).arg(m_requests_per_second);
  //system(a.toStdString().c_str());
  if (m_counter == m_number_of_requests ) exit();
  emit doRequest();
}
