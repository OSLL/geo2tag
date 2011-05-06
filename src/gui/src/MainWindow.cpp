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
/*! ---------------------------------------------------------------
 *
 *
 * \file MainWindow.cpp
 * \brief MainWindow implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include <QDebug>
#include "MainWindow.h"
#include "CentralWidget.h"

namespace GUI
{

  //TODO, kkv play with flags to get better appearence
  MainWindow::MainWindow() : QMainWindow(NULL /*, Qt::Desktop*/)
  {
    loginWindow = new LoginWindow(this);
    QObject::connect(loginWindow,SIGNAL(onSucsess(QString)),this,SLOT(onActivate(QString)));
    QObject::connect(loginWindow,SIGNAL(onSucsess(QString)), &GUI::OnLineInformation::getInstance(), SLOT(setAuthToken(QString)));
    #ifdef DESKTOP_STYLE
    loginWindow->show();
    #else
    loginWindow->showMaximized();
    #endif

    setWindowTitle("wikigps");
    setCentralWidget(new CentralWidget(this));
    createMenu();
    qDebug() << "centralWidget";
    setHidden(true);
  }

  void MainWindow::createMenu()
  {
    createActions();

    menuBar()->addAction(m_actionViewMap);
    // menuBar()->addAction(m_actionViewFeed);
    menuBar()->addAction(m_actionSelectChannels);
    menuBar()->addAction(m_actionAddContent);
    // menuBar()->addAction(m_actionEditRadius);
    //  menuBar()->addAction(m_actionShowDescription);
    menuBar()->addAction(m_actionViewOptions);
    menuBar()->addAction(m_logout);

    connect(m_actionViewMap, SIGNAL(triggered()), centralWidget(), SLOT(switchMap()));
    //connect(m_actionViewFeed, SIGNAL(triggered()), centralWidget(), SLOT(switchFeed()));
    connect(m_actionSelectChannels, SIGNAL(triggered()), centralWidget(), SLOT(switchChannel()));
    connect(m_actionAddContent,     SIGNAL(triggered()), centralWidget(), SLOT(switchEditor()));
    connect(m_actionEditRadius,     SIGNAL(triggered()), centralWidget(), SLOT(setRadius()));
    //    connect(m_actionShowDescription,SIGNAL(triggered()), centralWidget(), SLOT(showChannelInfo()));
    connect(m_actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(m_actionViewOptions, SIGNAL(triggered()), centralWidget(), SLOT(switchOptions()));
    connect(m_logout, SIGNAL(triggered()), this, SLOT(onDeactivate()));

  }

  void MainWindow::createActions()
  {
    m_actionViewMap = new QAction("Map",menuBar());
    m_actionViewFeed = new QAction("Feed",this);
    m_actionQuit = new QAction("Quit",this);
    m_actionSelectChannels = new QAction("Channels",this);
    m_actionEditRadius = new QAction("Set radius",this);
    m_actionShowDescription = new QAction("Channel info",this);
    m_actionAddContent = new QAction("Add",this);
    m_actionViewOptions = new QAction("Options", this);
    m_logout = new QAction("Logout", this);
  }

  void MainWindow::addContent()
  {
    qDebug() << "add";
  }

  void MainWindow::onActivate(QString)
  {
    loginWindow->setHidden(true);
    setHidden(false);
  }

  void MainWindow::onDeactivate()
  {
    loginWindow->setHidden(false);
    setHidden(true);
    OnLineInformation::getInstance().setAuthToken(QString(""));
  }

}                                       // namespace GUI


/* ===[ End of file ]=== */
