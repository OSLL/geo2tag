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
 * \file LoginThread.cpp
 * \brief LoginThread implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "LoginThread.h"
#include "ErrnoTypes.h"
#include <QDebug>
#include <QEventLoop>
#include "defines.h"

int LoginThread::m_counter=0;
int LoginThread::m_number_of_requests=-1;

LoginThread::LoginThread()
{
  m_startSession = new LoginQuery();
  connect(this,SIGNAL(doRequest()),this, SLOT(sendRequest()));
  connect(m_startSession,SIGNAL(connected()),this, SLOT(responseRecieved()));
  connect(m_startSession,SIGNAL(errorOccured(int)),this, SLOT(responseRecieved()));
}

LoginThread::~LoginThread()
{
  delete m_startSession;
}

void LoginThread::run()
{
 emit doRequest();
 exec();
}

int LoginThread::getCounter() 
{
  return m_counter;
}

void LoginThread::incCounter()
{
  m_counter++;
}

void LoginThread::sendRequest()
{

  srand(time(NULL));
  m_startSession->setQuery(DEFAULT_USER_NAME,DEFAULT_USER_PASSWORD);
  m_sendTime = QDateTime::currentDateTime();
  m_startSession->doRequest();
}

void LoginThread::responseRecieved()
{
  incCounter();
  qDebug() << getCounter() << " " << m_sendTime.msecsTo(QDateTime::currentDateTime()) << " " << m_startSession->getErrno();
  if (m_counter == m_number_of_requests ) exit();
  emit doRequest();
}
