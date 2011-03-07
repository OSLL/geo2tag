#include <QTimer>
#include <QDateTime>
#include "MainWindow.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_isServiceStarted(false),
    m_daemon(new QTcpSocket(this)),
    m_device(new QTextStream(m_daemon)),
    m_message("==begin of log==")
{
    setupUi(this);
    connect(startButton, SIGNAL(clicked()), SLOT(startButtonClicked()));
    connect(logButton, SIGNAL(clicked()), SLOT(logButtonClicked()));
    connect(settingsButton, SIGNAL(clicked()), SLOT(settingsButtonClicked()));
    connect(m_daemon,SIGNAL(readyRead()), SLOT(readData()));

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), SLOT(checkDaemon()));
    timer->start(5000);

    m_optionsWidget = new OptionsWidget("tracker",this);
    m_logWidget = new LogWidget(this);
    m_stackedWidget->addWidget(m_optionsWidget);
    m_stackedWidget->addWidget(m_logWidget);
    connect(m_optionsWidget, SIGNAL(done()), SLOT(moveToFirstPage()));
    connect(m_logWidget, SIGNAL(done()), SLOT(moveToFirstPage()));
    connect(m_optionsWidget, SIGNAL(cancel()), SLOT(moveToFirstPage()));
    //    setStyleSheet( "background-color: rgba( 255, 255, 255, 0% );" );
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::startButtonClicked()
{
    m_isServiceStarted = !m_isServiceStarted;
    if(!m_isServiceStarted)
    {
        startButton->setIcon(QIcon(":/images/start-256.png"));
        (*m_device) << "stop ";
    }
    else
    {
        startButton->setIcon(QIcon(":/images/stop-256.png"));
        (*m_device) << "start ";
    }

}

void MainWindow::logButtonClicked()
{
    m_stackedWidget->setCurrentWidget(m_logWidget);
}

void MainWindow::settingsButtonClicked()
{
    m_stackedWidget->setCurrentWidget(m_optionsWidget);
}

void MainWindow::moveToFirstPage()
{
    m_stackedWidget->setCurrentIndex(0);
}

void MainWindow::updateState()
{
    //requesting status
    if(m_daemon->state() >= QAbstractSocket::ConnectedState)
    {
        (*m_device) << "status ";
        m_device->flush();
    }
}

void MainWindow::readData()
{
    m_message += m_device->readAll();
    QRegExp statusExp("<status>(\\S)</status>");
    QRegExp logExp("<status>(\\S)</status>");
    //    int pos = 0;
    // split recieved message for many parts, process last and clean
    QStringList commands=m_message.split(" ",QString::SkipEmptyParts); 
    qDebug() << "recieved from trackerDaemon " << m_message;
    /* while( (pos = statusExp.indexIn(m_message,0)) != -1)
    {
        int length = statusExp.matchedLength();
        QString status = statusExp.cap(1);
        if(status == "started"){
  qDebug() << "text now " <<m_daemonIndicator->text();
            m_daemonIndicator->setText("Tracker runned");
            m_isServiceStarted = true;
 }
        if(status == "stoped"){
            m_daemonIndicator->setText("Tracker stopped");
            m_isServiceStarted = false;
 }
        m_message.remove(pos,length);
    }
    pos = 0;
    while( (pos = logExp.indexIn(m_message,0)) != -1)
    {
        int length = statusExp.matchedLength();
        QString logs = statusExp.cap(1);
        m_logWidget->addToLog(logs);
        m_message.remove(pos,length);
    }*/
    if (commands.last().indexOf("lastCoords_")!=-1){
        QString buf=commands.last();
        m_lastCoord = buf.right(buf.size()-QString("lastCoords_").size());
        //            m_daemonIndicator->setText(QString("Mark placed at ")+coords);o
        m_daemonIndicator->setText("Tracker running");
        m_logWidget->addToLog(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss.zzz")+": mark placed at "+m_lastCoord);
        qDebug() << "text now " <<m_daemonIndicator->text();
        m_isServiceStarted = true;
    }
    if (commands.last()=="stoped"){
        m_daemonIndicator->setText("Tracker stopped");
        m_isServiceStarted = false;
    }
    m_message.clear();

}

void MainWindow::checkDaemon()
{
    if(m_daemon->state() < QAbstractSocket::ConnectedState)
    {
        m_daemon->connectToHost("127.0.0.1", 31234);
    }
    if(m_daemon->state() >= QAbstractSocket::ConnectedState)
    {
        updateState();
    }
    else
        m_logWidget->addToLog(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss.zzz")+": can't connect to daemon");
}
