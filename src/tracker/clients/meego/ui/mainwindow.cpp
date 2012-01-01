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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(&m_client,SIGNAL(authentificated()),SLOT(on_authentificated()));
  connect(&m_client,SIGNAL(error(QString)),SLOT(on_error(string)));
  ui->log_edit->append("Error log");

}


MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_auth_button_pressed()
{
  if (!m_client.isAuthentificated())
    m_client.auth(ui->loginEdit->text(),ui->passEdit->text());

}


void MainWindow::on_startstopButton_pressed()
{
  if (m_client.isTracking())
  {
    m_client.stopTrack();
    ui->startstopButton->setText("Start tracking");
  }
  else
  {
    m_client.startTrack();
    ui->startstopButton->setText("Stop tracking");
  }
}


void MainWindow::on_authentificated()
{
  ui->auth_button->setEnabled(false);
}


void MainWindow::on_error(QString error)
{
  // qDebug() << "Error in lib occured";
  ui->log_edit->append(QDateTime::currentDateTimeUtc().toString()+":" + error);
}


void MainWindow::on_nameEdit_textChanged(QString name )
{
  m_client.setDispName(name);
}
