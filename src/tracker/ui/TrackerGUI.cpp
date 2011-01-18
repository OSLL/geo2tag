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

    m_statusTimer = new QTimer(this);
    m_statusTimer->setInterval(60 * 1000);
    connect(m_statusTimer, SIGNAL(timeout()), this, SLOT(updateStatus()));

    updateStatus();
}

void TrackerGUI::onStartTriggered()
{
    qDebug() << "start";
    DaemonManager::getInstance().start();
    updateStatus();
}

void TrackerGUI::onStopTriggered()
{
    qDebug() << "stop";
    DaemonManager::getInstance().stop();
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
    QDateTime now = QDateTime::currentDateTime();

    if (DaemonManager::getInstance().lastStatusModification().secsTo(now) < 60)
    {
        struct Status status = DaemonManager::getInstance().getStatus();
        if (status.status == "Ok")
        {
            QString message = status.datetime.toString("hh:mm:ss dd.MM.yyyy")
                              + ": " + status.status + ". " +
                              status.description;
            qDebug() << message;
            m_logWidget->addToLog(message);
            m_status->setIcon(QIcon(":/images/start"));
        }
        else
        {
            QString message = status.datetime.toString("hh:mm:ss dd.MM.yyyy")
                              + ": " + status.status + ". " +
                              status.description;
            qDebug() << message;
            m_logWidget->addToLog(message);
            m_status->setIcon(QIcon(":/images/error"));
        }
    }
    else
    {
        QString message = QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy")
                          + " Status hasn't been updated within "
                            "last 60 secs -> daemon isn't working";
        qDebug() << message;
        m_logWidget->addToLog(message);
        m_status->setIcon(QIcon(":/images/stop"));
    }

    m_statusTimer->start();
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
