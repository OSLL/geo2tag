#include "MainWindow.h"
#include <QDebug>
#include <QMenuBar>
MainWindow::MainWindow(QWidget* parent):
         QMainWindow(parent)
{
  m_stackedWidget = new QStackedWidget(this);
  m_map = new LightMap(this);
  QVBoxLayout *layout = new QVBoxLayout;
  m_menu = new QMenu(this);
  m_settingsAction = new QAction("Settings...",this);
  connect(m_settingsAction,SIGNAL(triggered()),this,SLOT(settings()));
  m_optWidget = new OptionsWidget("observer");
  connect(m_optWidget,SIGNAL(done()),this,SLOT(settingsDone()));
  connect(m_optWidget,SIGNAL(cancel()),this,SLOT(settingsDone()));
//init menu, add Optwidget into it
  m_menu=menuBar()->addMenu("Options");
  m_menu->addAction(m_settingsAction);
  m_map->setCenter( 60 , 30);
  m_stackedWidget->addWidget(m_map);
  m_stackedWidget->addWidget(m_optWidget);
  setCentralWidget(m_stackedWidget);
//  layout->addWidget(m_menu);
 //layout->addWidget(m_map);
//  setLayout(layout);
  m_thread = new DataThread(m_map); 
  m_thread->start();
  resize(600,300);
}

void MainWindow::settings()
{
  qDebug() << "MainWindow::settings()";
  m_stackedWidget->setCurrentWidget(m_optWidget);
}

void MainWindow::settingsDone()
{
  m_stackedWidget->setCurrentWidget(m_map);
}
