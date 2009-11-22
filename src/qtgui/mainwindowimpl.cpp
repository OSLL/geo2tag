//TODO delete
#include <QtGui/QMessageBox>

#include "mainwindowimpl.h"

MainWindowImpl::MainWindowImpl() : QMainWindow(NULL)
{
    ui.setupUi(this);

    feedView = new FeedViewImpl(this);
    mapView  = new MapViewImpl(this);
    webView  = new WebViewImpl(this);

    layout= new QStackedLayout(this);
    QWidget *cw = new QWidget(this);
    cw->setLayout(layout);
    layout->addWidget(feedView);
    layout->addWidget(mapView);
    layout->addWidget(webView);

    setCentralWidget(cw);

    connect(ui.actionShowFeed,SIGNAL(triggered()),this,SLOT(showFeed()));
    connect(ui.actionShowWeb,SIGNAL(triggered()),this,SLOT(showWeb()));
    connect(ui.actionShowMap,SIGNAL(triggered()),this,SLOT(showMap()));
}

MainWindowImpl::~MainWindowImpl()
{
}

void MainWindowImpl::showFeed()
{
    layout->setCurrentWidget(feedView);
}

void MainWindowImpl::showMap()
{
    layout->setCurrentWidget(mapView);
}

void MainWindowImpl::showWeb()
{
    layout->setCurrentWidget(webView);
}

