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

#ifndef DATATHREAD_H
#define DATATHREAD_H

#include <QThread>
#include <QSettings>
#include "MapScene.h"

#include "LoginQuery.h"
#include "LoadTagsQuery.h"

class DataThread : public QThread
{
  Q_OBJECT

    QSettings m_settings;
  // here marks are stored
  DataChannels m_marks;
  QSharedPointer<common::User> m_user;
  MapScene * m_map;

  // this field needs because query is asynchronous
  LoginQuery * m_loginQuery;
  // this field needs because query is asynchronous
  LoadTagsQuery * m_rssQuery;

  bool m_isConnected;
  bool m_requestIsSent;

  void run();

  protected slots:
    void onConnected();
    void onMarksGotten();
    void onGetFeed();
    void onError(QString);

  public:
    DataThread(MapScene * map, QObject *parent = 0);

    signals:
    void getFeed();
  public slots:

};
// DATATHREAD_H
#endif
