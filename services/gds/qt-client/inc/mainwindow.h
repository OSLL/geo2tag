#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include <QStackedWidget>
#include <QSharedPointer>

#include "GDSService.h"
#include "LoginWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QSharedPointer<GDSService> m_GDSService;

    // GUI
    QStackedWidget *m_stackedWidget;
    LoginWidget *m_loginWidget;

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

private:
    void initGUI();
};

#endif // MAINWINDOW_H
