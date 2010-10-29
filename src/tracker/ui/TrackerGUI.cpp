#include "TrackerGUI.h"

#include <QMenuBar>
#include <QMenu>
#include <QTcpSocket>
#include <QHostAddress>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Status.h"
#include "DaemonManager.h"

#define DAEMON_PORT 34243

TrackerGUI::TrackerGUI(QWidget *parent) :
        QMainWindow(parent)
{
//    /*
//     * Create RequestSender to interact with daemon
//     */
//    QTcpSocket *socket = new QTcpSocket(this);
//    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
//    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
//    socket->connectToHost(QHostAddress::LocalHost, DAEMON_PORT);

//////    socket->close();
//    m_requestSender = new RequestSender(socket, this);

//    /*
//     * Create menu
//     * There are two type of menu:
//     * 1. MenuBar with Actions
//     * 2. MenuBar contains Menu and Menu contains Actions
//     * One of them is uncommented.
//     */
//    m_startAction = new QAction("Start", this);
//    m_stopAction = new QAction("Stop", this);
//    m_optionsAction = new QAction("Options", this);
//    m_logAction = new QAction("Log", this);
//    /*
//    QMenu *fileMenu = this->menuBar()->addMenu("File");
//    fileMenu->addAction(m_startAction);
//    fileMenu->addAction(m_stopAction);
//    fileMenu->addAction(m_optionsAction);
//    fileMenu->addAction(m_logAction);
//    */
//    ///*
//    this->menuBar()->addAction(m_startAction);
//    this->menuBar()->addAction(m_stopAction);
//    this->menuBar()->addAction(m_optionsAction);
//    this->menuBar()->addAction(m_logAction);
//    //*/
//    connect(m_startAction, SIGNAL(triggered()),
//            this, SLOT(onStartTriggered()));
//    connect(m_stopAction, SIGNAL(triggered()),
//            this, SLOT(onStopTriggered()));
//    connect(m_optionsAction, SIGNAL(triggered()),
//            this, SLOT(switchOptions()));
//    connect(m_logAction, SIGNAL(triggered()),
//            this, SLOT(switchLog()));

    /*
     * Create title widget
     */
    m_titleWidget = new QWidget();
    m_startButton = new QPushButton(m_titleWidget);
    m_startButton->setIcon(QIcon(":/images/start.png"));
    m_startButton->setFlat(true);
    m_stopButton = new QPushButton(m_titleWidget);
    m_stopButton->setIcon(QIcon(":/images/stop.png"));
    m_stopButton->setFlat(true);
    m_optionsButton = new QPushButton(m_titleWidget);
    m_optionsButton->setIcon(QIcon(":/images/options.png"));
    m_optionsButton->setFlat(true);
    m_logButton = new QPushButton(m_titleWidget);
    m_logButton->setIcon(QIcon(":/images/log.png"));
    m_logButton->setFlat(true);
    QVBoxLayout *buttonsLayout = new QVBoxLayout();
    buttonsLayout->addWidget(m_startButton);
    buttonsLayout->addWidget(m_stopButton);
    buttonsLayout->addWidget(m_optionsButton);
    buttonsLayout->addWidget(m_logButton);
    buttonsLayout->addStretch();
    m_status = new QPushButton();
    m_status->setIcon(QIcon(":/images/stop"));
    m_status->setFlat(true);
    m_status->setIconSize(QSize(100, 100));
    m_status->setDisabled(true);
    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addLayout(buttonsLayout);
    titleLayout->addWidget(m_status);
    m_titleWidget->setLayout(titleLayout);

    /*
     * Create and fill stacked widget
     */
    m_stackedWidget = new QStackedWidget();
    m_stackedWidget->addWidget(m_titleWidget);
    if (m_titleWidget->parent() == 0)
    {
        qDebug() << "parent was'n found";
    }
    m_stackedWidget->addWidget(m_optionsWidget = new OptionsWidget(this));
    m_stackedWidget->addWidget(m_logWidget = new LogWidget(this));
    m_stackedWidget->setCurrentWidget(m_titleWidget);
    this->setCentralWidget(m_stackedWidget);
    connect(m_optionsWidget, SIGNAL(done()),
            this, SLOT(onOptionsWidgetDone()));
    connect(m_logWidget, SIGNAL(done()),
            this, SLOT(onLogWidgetDone()));
    connect(m_startButton, SIGNAL(clicked()), this, SLOT(onStartTriggered()));
    connect(m_stopButton, SIGNAL(clicked()), this, SLOT(onStopTriggered()));
    connect(m_optionsButton, SIGNAL(clicked()), this, SLOT(switchOptions()));
    connect(m_logButton, SIGNAL(clicked()), this, SLOT(switchLog()));

    updateStatus();

}

void TrackerGUI::onStartTriggered()
{
    qDebug() << "start";
    // TODO : send "start" to daemon
    DaemonManager::getInstance().start();
    updateStatus();
}

void TrackerGUI::onStopTriggered()
{
    qDebug() << "stop";
    DaemonManager::getInstance().stop();
    // TODO : send "stop" to daemon
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
    // TODO : check status file
//    struct Status status = m_requestSender->status();
//    QString newStatus;
//    if (status.valid)
//    {
//        newStatus = status.datetime.toString("dd.MM.yyyy hh:mm:ss") + " " +
//                    status.status + "<br>: " +
//                    status.description;
//    }
//    else
//    {
//        newStatus = "Can't connect to daemon";
//    }
//    m_status->setText(newStatus);
//    m_logWidget->setLog(newStatus + m_logWidget->getLog());
}

void TrackerGUI::connected(){
    qDebug() << "Connected to daemon!";
}

void  TrackerGUI::displayError(QAbstractSocket::SocketError socketError){
    switch (socketError) {
         case QAbstractSocket::RemoteHostClosedError:{
            qDebug() << "RemoteHostClosedError";
            break;
         }
         case QAbstractSocket::HostNotFoundError: {
             qDebug() << "HostNotFoundError";
             break;
         }
         case QAbstractSocket::ConnectionRefusedError:{
             qDebug() << "ConnectionRefusedError";
             break;
         }
    }

}
