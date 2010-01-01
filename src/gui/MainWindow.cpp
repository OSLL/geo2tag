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
    setWindowTitle("wikigps");
    setCentralWidget(new CentralWidget(this));
    createMenu();
    qDebug() << "centralWidget";
  }

  void MainWindow::createMenu()
  {
    createActions();

    menuBar()->addAction(m_actionViewMap);
   // menuBar()->addAction(m_actionViewFeed);
    menuBar()->addAction(m_actionSelectChannels);
    menuBar()->addAction(m_actionAddContent);
    menuBar()->addAction(m_actionEditRadius);
  //  menuBar()->addAction(m_actionShowDescription);

    connect(m_actionViewMap, SIGNAL(triggered()), centralWidget(), SLOT(switchMap()));
    //connect(m_actionViewFeed, SIGNAL(triggered()), centralWidget(), SLOT(switchFeed()));
    connect(m_actionSelectChannels, SIGNAL(triggered()), centralWidget(), SLOT(switchChannel()));
    connect(m_actionAddContent,     SIGNAL(triggered()), centralWidget(), SLOT(switchEditor()));
    connect(m_actionEditRadius,     SIGNAL(triggered()), centralWidget(), SLOT(setRadius()));
//    connect(m_actionShowDescription,SIGNAL(triggered()), centralWidget(), SLOT(showChannelInfo()));
    connect(m_actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    
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
  }

  void MainWindow::addContent()
  {
    qDebug() << "add";
  }


} // namespace GUI

/* ===[ End of file ]=== */
