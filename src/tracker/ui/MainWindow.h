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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTcpSocket>
#include <QTextStream>
#ifndef Q_WS_MAEMO_5
#include "ui_MainWindow.h"
#else
#include "ui_MainWindow_maemo.h"
#endif
#include "LogWidget.h"
#include "OptionsWidget.h"
#include "AboutWidget.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

    bool m_isServiceStarted;
  //  QTcpSocket *m_daemon;
  //  QTextStream *m_device;
  //  QString m_lastCoord;
  // contain data, received from daemon
  //  QString m_message;

  LogWidget * m_logWidget;
  OptionsWidget * m_optionsWidget;
  AboutWidget * m_aboutWidget;

  void updateState();

  private slots:
    void startButtonClicked();
    void logButtonClicked();
    void doneButtonClicked();
    void settingsButtonClicked();
    void aboutButtonClicked();

    void checkDaemon();
    void readData();

    void restartDaemon();
    void moveToFirstPage();

  public:
    explicit MainWindow(QWidget *parent = 0);

  protected:
    void changeEvent(QEvent *e);
};
// MAINWINDOW_H
#endif
