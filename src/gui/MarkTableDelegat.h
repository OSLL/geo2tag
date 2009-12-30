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

namespace GUI
{

  class ListModel : public QStandardItemModel
  {
      Q_OBJECT;

      CHandlePtr<common::DataMarks> m_data;
  
  public:
      ListModel(QObject* parent) : QStandardItemModel(common::DbSession::getInstance().getMarks()->size(),2,parent), m_data(common::DbSession::getInstance().getMarks())
      {
      }

  		virtual ~ListModel() {}
  
      int rowCount(const QModelIndex &/*parent = QModelIndex()*/) const
      {
        return m_data->size();
      }
      
      int columnCount ( const QModelIndex & /*parent = QModelIndex()*/ ) const
      {
        return 2;
      }

      void setDescription(int row, const std::string& data)
      {
        (*m_data)[row]->setDescription(data);
      }

      QVariant data(const QModelIndex &index, int role) const
      {
      	if ( Qt::DisplayRole == role )
      	{
          QString value="?";
          if(index.column() == 0 )
          {
      		 value = (*m_data)[index.row()]->getLabel().c_str();
          } else
          {
            value = (*m_data)[index.row()]->getDescription().c_str();
          }
      		return value;
      	}
      	return QVariant();
      }
   
   public slots:

      void layoutUpdate()
      {
        setRowCount(m_data->size());
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
      m_model->setHeaderData(1, Qt::Horizontal, tr("Mark message")); 
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
#if 0
    // zps commented this code. I think currently we don't need for it.
    bool event(QEvent* e){
      QKeyEvent* k=dynamic_cast<QKeyEvent*>(e);
      if (k && k->type()==6 ){
        if(k->key()==Qt::Key_Equal)
        { 
           
          CHandlePtr<common::DataMarks> marks = common::DbSession::getInstance().common::DbSession::getInstance().getMarks();
          std::ostringstream s; 
          s << "Label " << 'A'+(char)(marks->size());
          CHandlePtr<common::DataMark> m = common::DataMark::createMark(
                        common::GpsInfo::getInstance().getLatitude(),
                        common::GpsInfo::getInstance().getLongitude(), 
                        s.str(),
                        "", ""/*unknown/undefined url*/  );



           common::DbSession::getInstance().storeMark(m);
           dynamic_cast<ListModel*>(m_model)->layoutUpdate();
          
        }
      	else 
          if (k->key()==Qt::Key_Minus) {
              qDebug()<< "MINUS";
          } 
    	}
     return 1;
    }

#endif    
    ListView(const ListView& obj);
    ListView& operator=(const ListView& obj);
		QStandardItemModel *m_model;


  }; // class ListView
  



  
} // namespace GUI

#endif //_MarkTableDelegat_H_C595B6B3_EF48_4F54_879C_E662D3876A5C_INCLUDED_

/* ===[ End of file  ]=== */
