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
/*----------------------------------------------------------------- !
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef GENERATORDAEMON_H_
#define GENERATORDAEMON_H_

#include <QObject>
#include <QThread>
#include <QList>
#include <QPointF>
#include <QSharedPointer>
#include "LoginQuery.h"
#include "WriteTagQuery.h"
#include <QFile>
class GeneratorDaemon: public QThread
{
  Q_OBJECT;

  // User gotten after auth
  QSharedPointer<User> m_user;

  // Track channelname
  QString m_channelName;

  // List, where marks are stored
  QList<QPointF> m_marks;

  // Read all marks from file, specified in constructor, to m_marks
  void readMarks();

  // This function will run thread
  void run();

  // Query needed to recieve auth_token
  LoginQuery * m_loginQuery;

  // Query needed to send marks
  WriteTagQuery * m_tagQuery;

  bool m_isConnected;
  public:
    GeneratorDaemon();
    GeneratorDaemon(const QString& channelName);
    ~GeneratorDaemon();
  private slots:
    void onTagAdded();
    void onConnected();
    void onError(QString error);
};
#endif
