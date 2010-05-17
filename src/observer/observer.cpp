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
    m_lastLogin= "";
    m_marks=makeHandle(new common::DataMarks);
    loginQuery=new GUI::LoginQuery;
    qDebug() << "starting" ;
    rssFeedQuery=new GUI::RSSFeedQuery;
    connect(rssFeedQuery,SIGNAL(responseReceived(CHandlePtr<common::DataMarks>& )),
           this,SLOT(updateData(CHandlePtr<common::DataMarks>&)));
    connect(this,SIGNAL(dataMarksGotten(CHandlePtr<common::DataMarks>& )),
           m_ui.m_mapArea,SLOT(updated(CHandlePtr<common::DataMarks>&)));
    connect(m_ui.m_updateButton, SIGNAL(clicked()), this, SLOT(doRequest()));
    connect(m_ui.m_scale,SIGNAL(sliderMoved(int)),m_ui.m_mapArea,SLOT(scaleChanged(int)));
    connect(m_ui.m_users,SIGNAL(currentIndexChanged ( const QString&) ),this,SLOT(usersSelected(const QString&)));
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
    int lastIndex=-1;
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
	if (m_ui.m_user->text()==(*i)->getUser()->getLogin().c_str()) lastIndex=list.length()-1;
    }
    if (m_list != list) {
	    m_ui.m_users->clear();
	    m_ui.m_users->addItems(list);
	    m_list=list;
    }
}
    emit dataUpdated();
    if (m_ui.m_user->text()!=m_lastLogin && lastIndex!=-1) {
        m_lastLogin=m_ui.m_user->text();		
	m_ui.m_users->setCurrentIndex(lastIndex);
    }
}

void Observer::tokenRecieved(QString status,QString auth_token)
{
    disconnect(loginQuery, 0, 0, 0);
    qDebug() << "auth_token recieved " << auth_token;
    rssFeedQuery->setQuery(auth_token,30.0,30.0,30000000.0,true);
    rssFeedQuery->doRequest();
    
}

void Observer::doRequest()
{
    qDebug() << "rssFeedQuery->doRequest()";
    rssFeedQuery->doRequest();
}

void Observer::buttonPushed()
{
	qDebug() << "UpdateButton pushed " << m_ui.m_user->text() << " passw "<< m_ui.m_password->text() ;
	if (m_lastLogin != m_ui.m_user->text()){
//	m_lastLogin = m_ui.m_user->text();
	loginQuery->setQuery(m_ui.m_user->text(),m_ui.m_password->text());
	connect(loginQuery,SIGNAL(responseReceived(QString, QString)), this,SLOT(tokenRecieved(QString,QString)));
        loginQuery->doRequest();
	}

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
