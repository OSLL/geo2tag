#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QGraphicsView>
#include "MapScene.h"
#include "DataThread.h"
#include "OptionsWidget.h"
#include "ViewOptsWidget.h"
class MainWindow:public QMainWindow
{
   Q_OBJECT

   MapScene * m_map;
   QGraphicsView * m_view;
   QMenu * m_menu;
   QAction * m_settingsAction;
   QAction * m_marksSettingsAction;
   QStackedWidget * m_stackedWidget;
   DataThread * m_thread;
   OptionsWidget * m_optWidget;
   ViewOptsWidget *m_marksOptWidget;

public:
   MainWindow(QWidget *parent = 0);

protected:
   void showEvent(QShowEvent * e);

private slots:
   void settings();
   void marksSettings();
   void settingsDone();
};

#endif

