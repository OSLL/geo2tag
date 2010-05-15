#include "observer.h"
#include <QMessageBox>
#include <QPainter>
#include "Picture.h"
#include "User.h"
#include <QDebug>
#include "MapWidget.h"
#include <QComboBox>
#include <sstream>
#include <QObject>

Observer::Observer() : QDialog(NULL)
{
    m_ui.setupUi(this);
    m_marks=makeHandle(new common::DataMarks);
    loginQuery=new GUI::LoginQuery;
    qDebug() << "starting" ;
    connect(m_ui.m_updateButton, SIGNAL(pressed()), this, SLOT(buttonPushed()));
    connect(this, SIGNAL(dataUpdated()), this, SLOT(updateView()));
    connect(this, SIGNAL(dataUpdated()), this, SLOT(updateList()));
    connect(loginQuery,SIGNAL(responseReceived(QString, QString)), this,SLOT(tokenRecieved(QString,QString)));
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
    m_marks=marks;
    QStringList list;
    for (common::DataMarks::iterator i=marks->begin();i!=marks->end();i++)
    {
//        tmp.user=QString((*i)->getUser()->getLogin().c_str());
//        tmp.position=QPointF((*i)->getLatitude(),(*i)->getLongitude());
//	qDebug() << "before time making";
//	tmp.date=QDateTime::fromString(formatTime((*i)->getTime().getTime(),"%d %b %Y %H:%M:%S").c_str());
//	qDebug() << "after time making";
//        m_data.push_back(tmp);
        list << QString((*i)->getUser()->getLogin().c_str());
    }
    if (m_list != list) {
	    m_ui.m_users->clear();
	    m_ui.m_users->addItems(list);
	    m_list=list;
    }
}
    emit dataUpdated();
}

void Observer::tokenRecieved(QString status,QString auth_token)
{
//When auth is success make Observer visible and create RSSFeedQuery
    disconnect(loginQuery, 0, 0, 0);
    qDebug() << "auth_token recieved " << auth_token;
    rssFeedQuery=new GUI::RSSFeedQuery(auth_token,30.0,30.0,30000000.0,true);
    connect(rssFeedQuery,SIGNAL(responseReceived(CHandlePtr<common::DataMarks>& )),
       this,SLOT(updateData(CHandlePtr<common::DataMarks>&))); 
    connect(this,SIGNAL(dataMarksGotten(CHandlePtr<common::DataMarks>& )),
       m_ui.m_mapArea,SLOT(updated(CHandlePtr<common::DataMarks>&)));
    connect(m_ui.m_updateButton, SIGNAL(clicked()), this, SLOT(doRequest()));
    connect(m_ui.m_scale,SIGNAL(sliderMoved(int)),m_ui.m_mapArea,SLOT(scaleChanged(int)));
    connect(m_ui.m_users,SIGNAL(currentIndexChanged ( const QString&) ),this,SLOT(usersSelected(const QString&)));
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

//	loginQuery->doRequest();
//}
}

//New slot were we create DataMarks from one DataMark that corresponds for text user name

void Observer::usersSelected(const QString & text)
{
CHandlePtr<common::DataMarks> mrs=makeHandle(new common::DataMarks);
//Code below
for (common::DataMarks::iterator i=m_marks->begin();i!=m_marks->end();i++)
{
	if ((*i)->getUser()->getLogin()==text.toStdString()){
		mrs->push_back((*i));
//		std::stringstream st;
//		st << (*i)->getTime();
		QString timeQ;
	 	qDebug() << "getTime";
		timeQ=formatTime((*i)->getTime().getTime(),"%d %B %Y %H:%M:%S").c_str();
                m_ui.m_statusBar->showMessage(QObject::tr(timeQ.toStdString().c_str()));
		emit dataMarksGotten(mrs);
		break;
	}
}
}
