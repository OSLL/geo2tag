#include <QDebug>
#include "LoginWindow.h"
#include <QMessageBox>
namespace GUI
{

LoginWindow::LoginWindow(QWidget * parent, Qt::WindowFlags f):QMainWindow(parent,f)
{
    QWidget *widget = new QWidget(this);
    m_form.setupUi(widget);
    setCentralWidget(widget);
    connect(m_form.m_login, SIGNAL(clicked()), SLOT(okPressed()));
    connect(&m_query,SIGNAL(responseReceived(QString, QString,QString)),this,SLOT(responseReceived(QString,QString,QString)));
    connect(&m_query,SLOT(onManagerSslErrors()),&m_query,SIGNAL(errorReceived()));
    connect(&m_query,SLOT(onReplyError(QNetworkReply::NetworkError)),&m_query,SIGNAL(errorReceived()));
    connect(&m_query,SIGNAL(errorReceived()),this,SLOT(errorReceived()));
}

void LoginWindow::okPressed()
{
  qDebug() << "ok Pressed, query";
  m_query.setQuery(m_form.m_userName->text(),m_form.m_password->text());
  m_query.doRequest();
}


void LoginWindow::responseReceived(QString status, QString auth_token,QString status_description)
{
    qDebug() << "received login query response";
    if(status=="Ok")
    {
        emit onSucsess(auth_token);
    }
    else
    {
        QMessageBox::information( this, "wikigps","Wrong user name or password.");
        //error
    }
}

void LoginWindow::errorReceived()
{
    QMessageBox::information(this, "wikigps", "Network error");
}

}
