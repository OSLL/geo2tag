/*  */
/*!
 * \file MarkTableDelegat.h
 * \brief Header of MarkTableDelegat
 * \todo add comment here
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */


#ifndef _MarkTableDelegat_H_C595B6B3_EF48_4F54_879C_E662D3876A5C_INCLUDED_
#define _MarkTableDelegat_H_C595B6B3_EF48_4F54_879C_E662D3876A5C_INCLUDED_

#include <sstream>
#include <QAbstractListModel>
#include <QStandardItemModel>
#include <QWidget>
#include <QTableView>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QTimer>
#include "DataMarks.h"
#include <QItemDelegate>
#include <QTableWidget>
#include <QLineEdit>
#include <QKeyEvent>
#include "GpsInfo.h"
#include "Handle.h"
#include "DbSession.h"
#include "User.h"
#include "GpsInfo.h"

namespace GUI
{

  class ListModel : public QStandardItemModel
  {
      Q_OBJECT;

      CHandlePtr<common::DataMarks> m_data;
      CHandlePtr<common::Channel> m_currentChannel;
      size_t m_size; 
  public:
      ListModel(QObject* parent) : QStandardItemModel(common::DbSession::getInstance().getMarks()->size(),3,parent), m_data(common::DbSession::getInstance().getMarks())
      {
        m_size = 0;
      }

  		virtual ~ListModel() {}
  
      int rowCount(const QModelIndex &/*parent = QModelIndex()*/) const
      {
        return m_size;
      }
      
      int columnCount ( const QModelIndex & /*parent = QModelIndex()*/ ) const
      {
        return 3;
      }

      CHandlePtr<common::Channel> getCurrentChannel() const
      {
        return m_currentChannel;
      }

      void setDescription(int row, const std::string& data)
      {
        size_t c=0,i=0;
        for(; i<m_data->size(); i++)
        {
          if((*m_data)[i]->getChannel() == m_currentChannel)
            c++;
          if((c-1)==row)
            break;
        }

        (*m_data)[i]->setDescription(data);
      }

      QVariant data(const QModelIndex &index, int role) const
      {
      	if ( Qt::DisplayRole == role )
      	{
          QString value="?";
          size_t c=0,i=0;
          for(; i<m_data->size(); i++)
          {
            if((*m_data)[i]->getChannel() == m_currentChannel)
              c++;
            if((c-1)==index.row())
              break;
          }

          switch(index.column())
          {
          case 0:
      		 value = (*m_data)[i]->getLabel().c_str();
           break;
           
          case 1:
            value = (*m_data)[i]->getDescription().c_str();
            break;
            
          case 2:
            value = (*m_data)[i]->getUser()->getLogin().c_str();
            break;            
            
          default:
            break;        
          }  
          
      		return value;
      	}
      	return QVariant();
      }
   
   public slots:

      void layoutUpdate(CHandlePtr<common::Channel> channel = CHandlePtr<common::Channel>())
      {
        if(channel!=0)
          m_currentChannel=channel;

        double longitude = common::GpsInfo::getInstance().getLongitude();
        double latitude = common::GpsInfo::getInstance().getLatitude();
        size_t size=0;
        for(size_t i=0; i<m_data->size(); i++)
        {
          CHandlePtr<common::DataMark> mark = (*m_data)[i];
          if(mark->getChannel()              == m_currentChannel && 
             mark->getChannel()->getRadius() >  common::DataMark::getDistance(latitude, longitude,mark->getLatitude(), mark->getLongitude()))
           size++;
        }
        m_size = size;
        setRowCount(size);
        emit layoutChanged();
      } 
  };

 /*!
   * Class description. May use HTML formatting
   *
   */
  class MarkTableDelegat : public QItemDelegate
  {
     Q_OBJECT;
  public:
    MarkTableDelegat(QWidget *parent =NULL) : QItemDelegate(parent)
    {
    }

    virtual ~MarkTableDelegat()
    {
    }

    int columnCount ( const QModelIndex & /*parent = QModelIndex()*/ ) const
    {
        return 2;
    }
  }; // class MarkTableDelegat
 
 /*!
   * Class description. May use HTML formatting
   *
   */
  class ListView : public QTableView
  {
		Q_OBJECT;
    QTimer *timer; // auto-updating timer
  public:
    ListView(QWidget *parent) : QTableView(parent) 
    {
      m_model = new ListModel(this);
			setModel(m_model);
      m_model->setHeaderData(0, Qt::Horizontal, tr("Map"));
      m_model->setHeaderData(1, Qt::Horizontal, tr("Mark text")); 
      m_model->setHeaderData(2, Qt::Horizontal, tr("author"));
      horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
      setItemDelegate(new MarkTableDelegat(this));
      timer = new QTimer(this);
      timer->setInterval(2*60*1000); // One time per 2 minutes
      connect(timer, SIGNAL(timeout()), m_model, SLOT(layoutUpdate()));
      timer->start();
    }

    virtual ~ListView() 
    {
    }

  private:    

    
    
    ListView(const ListView& obj);
    ListView& operator=(const ListView& obj);
		QStandardItemModel *m_model;


  }; // class ListView
  



  
} // namespace GUI

#endif //_MarkTableDelegat_H_C595B6B3_EF48_4F54_879C_E662D3876A5C_INCLUDED_

/* ===[ End of file  ]=== */
