#include <QEventLoop>
#include <QDebug>
#include "DataThread.h"


DataThread::DataThread(QObject *parent) :
    QThread(parent),  m_settings("osll","observer"), m_isConnected(false)
{
}



void DataThread::run()
{
    qDebug() << "thread started";
    QEventLoop eventLoop;
    QString login = m_settings.value("user").toString();
    QString password = m_settings.value("password").toString();
    if(login.isEmpty())
        login = "Mark";
    if(password.isEmpty())
        password = "test";

    m_loginQuery = new LoginQuery(login, password, parent());
    connect(m_loginQuery, SIGNAL(connected()), SLOT(onConnected()));
    connect(m_loginQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
    m_loginQuery->doRequest();
    for(;;)
    {
        if(m_isConnected)
        {
            qDebug() << "connected: auth_token=" << m_loginQuery->getUser()->getToken();
            //ToDo query RSS
        }
        eventLoop.processEvents(QEventLoop::ExcludeUserInputEvents, 1000);
        QThread::msleep(5000);
    }
}


void DataThread::onConnected()
{
    m_isConnected = true;
}

void DataThread::onError(QString /*message*/)
{

}
