#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H

#include <QtGui/QMainWindow>
#include <QtGui/QStackedWidget>

#include "ui_mainwindow.h"

#include "FeedViewImpl.h"
#include "MapViewImpl.h"
#include "WebViewImpl.h"

class MainWindowImpl : public QMainWindow
{
    Q_OBJECT;
public:
    MainWindowImpl();
    virtual ~MainWindowImpl();

public slots:
    void showFeed();
    void showMap();
    void showWeb();
private:
    Ui::MainWindow  ui;
    QStackedWidget *centralWidget;
    FeedViewImpl   *feedView;
    MapViewImpl    *mapView;
    WebViewImpl    *webView;

};

#endif // MAINWINDOWIMPL_H
