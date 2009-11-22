//TODO delete
#include <QtGui/QMessageBox>
#include "mainwindowimpl.h"

MainWindowImpl::MainWindowImpl() : QMainWindow(NULL)
{
    ui.setupUi(this);

    centralWidget = new QStackedWidget(this);

    feedView = new FeedViewImpl(this);
    mapView  = new MapViewImpl(this);
    webView  = new WebViewImpl(this);

    centralWidget->addWidget(feedView);
    centralWidget->addWidget(mapView);
    centralWidget->addWidget(webView);


    setCentralWidget(centralWidget);

    connect(ui.actionShowFeed,SIGNAL(triggered()),this,SLOT(showFeed()));
    connect(ui.actionShowWeb,SIGNAL(triggered()),this,SLOT(showWeb()));
    connect(ui.actionShowMap,SIGNAL(triggered()),this,SLOT(showMap()));
}

MainWindowImpl::~MainWindowImpl()
{
}

void MainWindowImpl::showFeed()
{
    centralWidget->setCurrentWidget(feedView);
}

void MainWindowImpl::showMap()
{
    centralWidget->setCurrentWidget(mapView);
}

void MainWindowImpl::showWeb()
{
    centralWidget->setCurrentWidget(webView);
}

