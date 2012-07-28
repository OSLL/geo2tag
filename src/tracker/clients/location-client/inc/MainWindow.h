#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QDeclarativeView>
#include "Client.h"
#include "ContactModel.h"

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
    ContactModel contactModel;

  public:
    explicit MainWindow(QWidget *parent = 0);

    signals:
    void trackingStarted();
  public slots:
    void onAuth(QString login, QString pass);
    void onReg(const QString &email, const QString &login, const QString &pass);
    void trackingOnOff();
    void changeSettings(int track_interval,bool permission);
    void onSubscribe(const QString &channelName);
    void unsubscribeChannel(const QString& channelName);
    void logout();

  private:
    void createMenus();
    void createActions();
  private slots:
    void shedulerSettings();
    void trackingSettings();
    void pathSettings();

};
#endif                                  // MAINWINDOW_H
