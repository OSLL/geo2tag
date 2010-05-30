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
#include <QTableWidget>
#include <QTableWidgetItem>

//I didn't find where it is and make my own GoogleScaleList
static double s_scales[]={83333.33,41666.67,21052.63,10526.32 ,5263.16,2500.00,1250.00,625.0,307.69,160.00,80.00,40.00,20.00,10.00,5.00,2.50,1.25,0.63,0.31,0.16};


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
    connect(m_ui.m_updateButton, SIGNAL(pressed()), this, SLOT(buttonPushed()));
    connect(m_ui.m_showAll, SIGNAL(pressed()), this, SLOT(showAllMarks()));
}



void Observer::updateData(CHandlePtr<common::DataMarks>& marks)
{
    if (rssFeedQuery){
    qDebug() << "updateData slot";
      m_marks=marks;
      int j=0;
      m_ui.m_tableWidget->clearContents();
      m_ui.m_tableWidget->setRowCount(marks->size());
      //Optimal scale finding:
      //    Find min and max Latitude and Longitude from marks. 
      //    Count width and height of rect that marks are take place
      //    Find optimal scale in wich all marks shown on a screen
      double maxLat=(*marks)[0]->getLatitude(),minLat=(*marks)[0]->getLatitude(),maxLon=(*marks)[0]->getLongitude(),minLon=(*marks)[0]->getLongitude();
      for (common::DataMarks::iterator i=marks->begin();i!=marks->end();i++,j++)
      {
        //Finding min and max Latitude and Longitude
        if ((*i)->getLatitude()<minLat) minLat=(*i)->getLatitude();
        if ((*i)->getLongitude()<minLon) minLon=(*i)->getLongitude();
        if ((*i)->getLatitude()>maxLat) maxLat=(*i)->getLatitude();
        if ((*i)->getLongitude()>maxLon) maxLon=(*i)->getLongitude();
        //Add items into m_tableWidget
        QTableWidgetItem* name=new QTableWidgetItem(QString((*i)->getUser()->getLogin().c_str()));
        QTableWidgetItem* mark=new QTableWidgetItem(QString((*i)->getLabel().c_str()));
        m_ui.m_tableWidget->setItem(j,0,name);
        m_ui.m_tableWidget->setItem(j,1,mark);
      }
      qDebug() << "Lat " << maxLat-minLat << " Lon "<<maxLon-minLon;
      //Count size of rect where all marks can be placed
      double height_= common::DataMark::getDistance(maxLat,maxLon,maxLat,minLon)*1000.;
      double width_ = common::DataMark::getDistance(maxLat,maxLon,minLat,maxLon)*1000.;
      qDebug() << "height " << height_ << " width "<< width_;
      qDebug() << "height " << height() << " width "<< width();
      //Choose optimal scale
      for (int i=19;i>=0;i--) 
        {
          qDebug() << "height " << (height_)/s_scales[i] << " width "<< (width_)/s_scales[i];
          if (height()>(height_)/s_scales[i] && width()>(width_)/s_scales[i]) {
            qDebug() << "Selected scale " <<  s_scales[i] ;
            //m_optScale will contain optimal scale value
            m_optScale=i;
            m_ui.m_scale->setValue(i);
            m_ui.m_mapArea->scaleChanged(i);
            break;
          }
        }
    }
    emit dataMarksGotten(marks);
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
	loginQuery->setQuery(m_ui.m_user->text(),m_ui.m_password->text());
	connect(loginQuery,SIGNAL(responseReceived(QString, QString)), this,SLOT(tokenRecieved(QString,QString)));
        loginQuery->doRequest();
	}

}
//Restore scale to optimal - set scale as optScale
void Observer::showAllMarks(){

  m_ui.m_scale->setValue(m_optScale);
  m_ui.m_mapArea->scaleChanged(m_optScale);
  }
