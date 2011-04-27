#include "MainWindow.h"
#include <QDebug>
#include <QMenuBar>
#include "defines.h"

MainWindow::MainWindow(QWidget* parent):
         QMainWindow(parent)
{
  m_stackedWidget = new QStackedWidget(this);
  m_map = new MapScene(this);//LightMap(this);
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
