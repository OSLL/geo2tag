#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QDeclarativeView>
#include "Client.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT
    private:
    QMenu *menu;

    QAction *shedulerAction;
    QAction *trackingAction;
    QAction *authAction;
    QAction *pathAction;

    QDeclarativeView* view;
    Client *client;

  public:
    explicit MainWindow(QWidget *parent = 0);

    signals:

  public slots:
  private:
    void createMenus();
    void createActions();
  private slots:
    void shedulerSettings();
    void trackingSettings();
    void pathSettings();

};
#endif                                  // MAINWINDOW_H
