#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include <QStackedWidget>
#include <QSharedPointer>
#include <QMenuBar>

#include "GDSService.h"
#include "LoginWidget.h"
#include "MainWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QSharedPointer<GDSService> m_GDSService;

    // GUI
    QStackedWidget *m_stackedWidget;
    LoginWidget *m_loginWidget;
    MainWidget *m_mainWidget;
    QMenuBar *m_menuBar;
    QMenu *m_menu;
    QAction *m_settingsAction;
    QAction *m_signOutAction;

public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Symbian and Fremantle.
    void setOrientation(ScreenOrientation orientation);
    void showExpanded();

public slots:
    void onLoginSignedIn(const QString& authToken);
    void onMainSignedOut();

    void testHideLogout();

private:
    void createActions();
    void createMenus();
    void initGUI();
};

#endif // MAINWINDOW_H
