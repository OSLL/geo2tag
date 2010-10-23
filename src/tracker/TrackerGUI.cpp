#include "TrackerGUI.h"

#include <QMenuBar>
#include <QMenu>
#include <QTcpSocket>
#include <QHostAddress>
#include "Status.h"

#define DAEMON_PORT 34243

TrackerGUI::TrackerGUI(QWidget *parent) :
        QMainWindow(parent)
{
    /*
     * Create RequestSender to interact with daemon
     */
    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress::LocalHost, DAEMON_PORT);
    socket->close();
    m_requestSender = new RequestSender(socket, this);

    /*
     * Create menu
     * There are two type of menu:
     * 1. MenuBar with Actions
     * 2. MenuBar contains Menu and Menu contains Actions
     * One of them is uncommented.
     */
    m_startAction = new QAction("Start", this);
    m_stopAction = new QAction("Stop", this);
    m_optionsAction = new QAction("Options", this);
    m_logAction = new QAction("Log", this);
    /*
    QMenu *fileMenu = this->menuBar()->addMenu("File");
    fileMenu->addAction(m_startAction);
    fileMenu->addAction(m_stopAction);
    fileMenu->addAction(m_optionsAction);
    fileMenu->addAction(m_logAction);
    */
    ///*
    this->menuBar()->addAction(m_startAction);
    this->menuBar()->addAction(m_stopAction);
    this->menuBar()->addAction(m_optionsAction);
    this->menuBar()->addAction(m_logAction);
    //*/
    connect(m_startAction, SIGNAL(triggered()),
            this, SLOT(onStartTriggered()));
    connect(m_stopAction, SIGNAL(triggered()),
            this, SLOT(onStopTriggered()));
    connect(m_optionsAction, SIGNAL(triggered()),
            this, SLOT(switchOptions()));
    connect(m_logAction, SIGNAL(triggered()),
            this, SLOT(switchLog()));

    /*
     * Create and fill stacked widget
     */
    m_stackedWidget = new QStackedWidget();
    m_stackedWidget->addWidget(m_titleWidget = new TitleWidget(this));
    m_stackedWidget->addWidget(m_optionsWidget = new OptionsWidget(this));
    m_stackedWidget->addWidget(m_logWidget = new LogWidget(this));
    m_stackedWidget->setCurrentWidget(m_titleWidget);
    this->setCentralWidget(m_stackedWidget);
    connect(m_optionsWidget, SIGNAL(done()),
            this, SLOT(onOptionsWidgetDone()));
    connect(m_logWidget, SIGNAL(done()),
            this, SLOT(onLogWidgetDone()));

    updateStatus();

}

void TrackerGUI::onStartTriggered()
{
    m_requestSender->start();
    switchTitle();
    updateStatus();
}

void TrackerGUI::onStopTriggered()
{
    m_requestSender->stop();
    switchTitle();
    updateStatus();
}

void TrackerGUI::switchTitle()
{
    m_stackedWidget->setCurrentWidget(m_titleWidget);
}

void TrackerGUI::switchOptions()
{
    m_stackedWidget->setCurrentWidget(m_optionsWidget);
}


void TrackerGUI::switchLog()
{
    m_stackedWidget->setCurrentWidget(m_logWidget);
}

void TrackerGUI::onOptionsWidgetDone()
{
    switchTitle();
}

void TrackerGUI::onLogWidgetDone()
{
    switchTitle();
}

void TrackerGUI::updateStatus()
{
    struct Status status = m_requestSender->status();
    QString newStatus;
    if (status.valid)
    {
        newStatus = status.datetime.toString("dd.MM.yyyy hh:mm:ss") + " " +
                    status.status + ": " +
                    status.description;
    }
    else
    {
        newStatus = "Can't connect to daemon";
    }
    m_titleWidget->setStatus(newStatus);
    m_logWidget->setLog(newStatus + m_logWidget->getLog());
}

