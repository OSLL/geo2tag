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

#include <QDebug>
#include <QObject>
#include <QCoreApplication>
#include "TrackerDaemon.h"

void myMessageHandler(QtMsgType type, const char *msg)
{
  QString txt;

  switch (type)
  {
    case QtDebugMsg:
      txt = QString("Debug: %1").arg(msg);
      break;
    case QtWarningMsg:
      txt = QString("Warning: %1").arg(msg);
      break;
    case QtCriticalMsg:
      txt = QString("Critical: %1").arg(msg);
      break;
    case QtFatalMsg:
      txt = QString("Fatal: %1").arg(msg);
      abort();
  }
  QFile outFile("/tmp/wikigpsTracker.log");
  outFile.open(QIODevice::WriteOnly | QIODevice::Append);

  QTextStream ts(&outFile);
  ts << QTime::currentTime().toString() << ":" << txt << endl;
}


int main(int c, char **v)
{
  QCoreApplication app(c,v);
  qInstallMsgHandler(myMessageHandler);

  qDebug() << "Application is started";

  TrackerDaemon daemon;

  daemon.run();

  return app.exec();
}
