#include "observer.h"
#include <QMessageBox>
#include <QPainter>
#include "Picture.h"
//#include "GoogleMapLoader.h"
#include "User.h"
//#include "MapLoaderFactory.h"
//#include "MapLoader.h"
#include <QDebug>
#include "MapWidget.h"


Observer::Observer() : QDialog(NULL)
{
    m_ui.setupUi(this);
    loginQuery=new GUI::LoginQuery;
    qDebug() << "starting" ;
//    connect(m_ui.m_updateButton, SIGNAL(clicked()), this, SLOT(updateData()));
    connect(m_ui.m_updateButton, SIGNAL(pressed()), this, SLOT(buttonPushed()));
    connect(this, SIGNAL(dataUpdated()), this, SLOT(updateView()));
    connect(this, SIGNAL(dataUpdated()), this, SLOT(updateList()));
}


void Observer::updateView()
{
    //QMessageBox::information(this,"Error", "Update view: not implemented");
}

void Observer::updateList()
{
  //  QMessageBox::information(this,"Error", "Update list: not implemented");
}

void Observer::updateData(CHandlePtr<common::DataMarks>& marks)
{
//    QMessageBox::information(this,"Error", "Update data: not implemented");

    //TODO: put update marks gere
    // 1. get marks
    // 2. fill vector m data
// Get dataMark 
// push it into m_data
    if (rssFeedQuery){
    qDebug() << "updateData slot";
    m_data.clear();
    MarkInfo tmp;
    for (common::DataMarks::iterator i=marks->begin();i!=marks->end();i++)
    {
        tmp.user=QString((*i)->getUser()->getLogin().c_str());
        tmp.position=QPointF((*i)->getLatitude(),(*i)->getLongitude());
	qDebug() << "before time making";
//	tmp.date=QDateTime::fromString(formatTime((*i)->getTime().getTime(),"%d %b %Y %H:%M:%S").c_str());
	qDebug() << "after time making";
        m_data.push_back(tmp);
    }
//    QPainter painter(m_ui.m_mapArea);
    //common::Picture picture = maps::MapLoaderFactory::getInstance(maps::MapLoader::GOOGLE)->getMapWithMarks(
//			0.0,0.0,12,m_ui.m_mapArea->rect().width(),m_ui.m_mapArea->rect().height(),*marks);
  //  QByteArray array(&(picture.getData()[0]), picture.getData().size());
//    painter.drawImage(m_ui.m_mapArea->rect(),QImage::fromData(array));
}
    emit dataUpdated();
}

void Observer::tokenRecieved(QString status,QString auth_token)
{
//When auth is success make Observer visible and create RSSFeedQuery
    disconnect(loginQuery);
    qDebug() << "auth_token recieved " << auth_token;
    rssFeedQuery=new GUI::RSSFeedQuery(auth_token,30.0,30.0,30000000.0,true);
    connect(rssFeedQuery,SIGNAL(responseReceived(CHandlePtr<common::DataMarks>& )),
       this,SLOT(updateData(CHandlePtr<common::DataMarks>&))); 
    connect(rssFeedQuery,SIGNAL(responseReceived(CHandlePtr<common::DataMarks>& )),
       m_ui.m_mapArea,SLOT(updated(CHandlePtr<common::DataMarks>&)));
    connect(m_ui.m_updateButton, SIGNAL(clicked()), this, SLOT(doRequest()));
    rssFeedQuery->doRequest();
}

void Observer::doRequest()
{
    qDebug() << "rssFeedQuery->doRequest()";
    rssFeedQuery->doRequest();
}

void Observer::buttonPushed()
{
//if (!rssFeedQuery){
	qDebug() << "UpdateButton pushed " << m_ui.m_user->text() << " passw "<< m_ui.m_password->text() ;
	loginQuery->setQuery(m_ui.m_user->text(),m_ui.m_password->text());
        loginQuery->doRequest();
	connect(loginQuery,SIGNAL(responseReceived(QString, QString)), this,SLOT(tokenRecieved(QString,QString)));
//	loginQuery->doRequest();
//}
}

