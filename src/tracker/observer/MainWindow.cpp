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

#include "MainWindow.h"
#include <QDebug>
#include <QMenuBar>
#include <QShowEvent>
#include "defines.h"

MainWindow::MainWindow(QWidget* parent):
QMainWindow(parent)
{
  m_stackedWidget = new QStackedWidget(this);
  m_map = new MapScene(this);           //LightMap(this);
  //kkv why?  QVBoxLayout *layout = new QVBoxLayout;
  m_menu = new QMenu(this);
  m_settingsAction = new QAction("Settings...",this);
  m_marksSettingsAction = new QAction("Marks view settings",this);
  connect(m_settingsAction,SIGNAL(triggered()),this,SLOT(settings()));
  connect(m_marksSettingsAction,SIGNAL(triggered()),this,SLOT(marksSettings()));
  m_optWidget = new OptionsWidget("observer");
  m_marksOptWidget = new ViewOptsWidget();
  connect(m_optWidget,SIGNAL(done()),this,SLOT(settingsDone()));
  connect(m_optWidget,SIGNAL(cancel()),this,SLOT(settingsDone()));
  connect(m_marksOptWidget,SIGNAL(cancel()),this,SLOT(settingsDone()));
  connect(m_marksOptWidget,SIGNAL(done()),this,SLOT(settingsDone()));
  //init menu, add Optwidget into it
  m_menu=menuBar()->addMenu("Options");
  m_menu->addAction(m_settingsAction);
  m_menu->addAction(m_marksSettingsAction);
  m_view = new QGraphicsView(this);
  m_view->setScene(m_map);
  m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_stackedWidget->addWidget(m_view);
  //TODO: Implement setCenter for MapScene
  //m_map->setCenter( DEFAULT_LATITUDE, DEFAULT_LONGITUDE);
  m_stackedWidget->addWidget(m_optWidget);
  m_stackedWidget->addWidget(m_marksOptWidget);
  setCentralWidget(m_stackedWidget);
  //  layout->addWidget(m_menu);
  //layout->addWidget(m_map);
  //  setLayout(layout);

  m_thread = new DataThread(m_map);
  m_thread->start();

  setWindowState(Qt::WindowMaximized);
  //resize(600,300);
}


void MainWindow::settings()
{
  qDebug() << "MainWindow::settings()";
  m_stackedWidget->setCurrentWidget(m_optWidget);
}


void MainWindow::marksSettings()
{
  qDebug() << "MainWindow::marksSettings()";
  m_stackedWidget->setCurrentWidget(m_marksOptWidget);
}


void MainWindow::settingsDone()
{
  m_stackedWidget->setCurrentWidget(m_view);
}


void MainWindow::showEvent(QShowEvent * e)
{
  if(e->type() == QShowEvent::Show)     // e must not be unused
    this->m_map->update_state();
}
