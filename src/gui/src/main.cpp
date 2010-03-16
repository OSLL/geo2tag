/*
 * Copyright 2010  OSLL osll@osll.spb.ru
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
#include <sstream>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include "MainWindow.h" 
#include "DbSession.h"
#include "GpsInfo.h"
#include "GoogleClientLogin.h"

int main(int c, char **v)
{
  QApplication 	app(c,v);

  try
  {
    common::DbSession::getInstance().loadData("Paul","test");
  }
  catch(ODBC::CException &x)
  {
    std::ostringstream s;
    s << x;
    qDebug() << s.str().c_str();
    QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("No database connection!!! Exiting..."));
    return 0;
  }
  catch(CException &x)
  {
    std::ostringstream s;
    s << x;
    qDebug() << s.str().c_str();
    QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr(x.getDescription().c_str()));
    return 0;
  }
  
  common::GpsInfo::getInstance(); //starting daemon and other stuff;


  GUI::MainWindow window;

#ifdef DESKTOP_STYLE
  window.show();
#else  
  window.showMaximized();
#endif  

  app.exec();

  try
  {
    common::DbSession::getInstance().saveData();
  }
  catch(ODBC::CException &x)
  {
    std::ostringstream s;
    s << x;
    qDebug() << s.str().c_str();
    QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Error during saving data... Some data can be lost."));
    return 0;
  }
  catch(CException &x)
  {
    std::ostringstream s;
    s << x;
    qDebug() << s.str().c_str();
    QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr(x.getDescription().c_str()));
    return 0;
  }

  return 0;

}

