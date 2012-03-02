#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include <QStackedWidget>
#include <QSharedPointer>
#include <QMenuBar>

#include "GDSService.h"
#include "LoginWidget.h"
#include "CreateAccountWidget.h"
#include "MainWidget.h"
#include "SettingsWidget.h"
#include "Settings.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QSharedPointer<GDSService> m_GDSService;
    bool m_isSignedIn;
    Settings m_settings;

    // GUI
    QStackedWidget *m_stackedWidget;
    LoginWidget *m_loginWidget;
    CreateAccountWidget *m_createAccountWidget;
    MainWidget *m_mainWidget;
    SettingsWidget *m_settingsWidget;
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
    void onCreateAccountRequested();
    void onCreateAccountFinished();
    void onSettingsSaved();
    void onSettingsCancelled();

    void onSettingsAction();
    void onSignOutAction();

private:
    void createActions();
    void createMenus();
    void initGUI();
};

#endif // MAINWINDOW_H
