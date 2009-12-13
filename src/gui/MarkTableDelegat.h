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
#include "DataMarks.h"
#include <QItemDelegate>
#include <QTableWidget>
#include <QLineEdit>
#include <QKeyEvent>
#include "defs.h"
#include "GpsInfo.h"

namespace GUI
{

 const char MARK_NAMES[]={"ABCDEFGHIKLMNOPRSTUVWXYZ"};

  class ListModel : public QStandardItemModel
  {
      Q_OBJECT;

      common::DataMarks& m_data;
  
  public:
      ListModel(QObject* parent) : QStandardItemModel(getMarks().size(),2,parent), m_data(getMarks())
      {
      }

  		virtual ~ListModel() {}
  
      int rowCount(const QModelIndex &/*parent = QModelIndex()*/) const
      {
        return m_data.size();
      }
      
      int columnCount ( const QModelIndex & /*parent = QModelIndex()*/ ) const
      {
        return 2;
      }

      void setDescription(int row, const std::string& data)
      {
        m_data[row]->setDescription(data);
      }

      QVariant data(const QModelIndex &index, int role) const
      {
      	if ( Qt::DisplayRole == role )
      	{
          QString value="?";
          if(index.column() == 0 )
          {
      		 value = m_data[index.row()]->getLabel().c_str();
          } else
          {
            value = m_data[index.row()]->getDescription().c_str();
          }
      		return value;
      	}
      	return QVariant();
      }

	void layoutUpdate(){
		setRowCount(m_data.size());
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

    int rowCount(const QModelIndex &/*parent = QModelIndex()*/) const
    {
        return 33;
    }
      
    int columnCount ( const QModelIndex & /*parent = QModelIndex()*/ ) const
    {
        return 2;
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model,  const QModelIndex &index) const
     {
        qDebug() << "setDATA";
        QString value = qobject_cast<QLineEdit*>(editor)->text();
        ListModel *mymodel = qobject_cast<ListModel*>(model);

        mymodel->setDescription(index.row(),value.toStdString());
     }

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */,  const QModelIndex &/*index*/) const
    {
       return new QLineEdit(parent);
    }

  }; // class MarkTableDelegat
 
 /*!
   * Class description. May use HTML formatting
   *
   */
  class ListView : public QTableView
  {
		Q_OBJECT;

  public:
    ListView(QWidget *parent) : QTableView(parent) 
    {
      m_model = new ListModel(this);
			setModel(m_model);
      m_model->setHeaderData(0, Qt::Horizontal, tr("Name"));
      m_model->setHeaderData(1, Qt::Horizontal, tr("Description")); 
      horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
      setItemDelegate(new MarkTableDelegat(this));
    }

    virtual ~ListView() 
    {
    }

  private:    
    bool event(QEvent* e){
      QKeyEvent* k=dynamic_cast<QKeyEvent*>(e);
      if (k && k->type()==6 ){
	if(k->key()==Qt::Key_Equal)
        { 
           qDebug()<< "PLUS";
           common::DataMark m;
           std::ostringstream s; 
           s << "Label " << 'A'+(char)(getMarks().size());
           m.setLabel(s.str());
           m.setDescription("");
           m.setLongitude(common::GpsInfo::getInstance().getLongitude());
           m.setLatitude(common::GpsInfo::getInstance().getLatitude());
           getMarks().push_back(makeHandle(new common::DataMark(m)));
           getMarks().save();
           
           dynamic_cast<ListModel*>(m_model)->layoutUpdate();
        }
	else 
          if (k->key()==Qt::Key_Minus) {
              qDebug()<< "MINUS";
          } 
	}
     return 1;
    }
    ListView(const ListView& obj);
    ListView& operator=(const ListView& obj);
		QStandardItemModel *m_model;


  }; // class ListView
  

  class MarkPane : public QWidget
  {
    ListView *m_list;
    Q_OBJECT;

  public:
    MarkPane(QWidget * parent) : QWidget(parent)
    {
  	  QHBoxLayout *vbl = new QHBoxLayout(this);

      m_list = new ListView(this);
  		vbl->addWidget(m_list);
      setLayout(vbl);
    }


    virtual ~MarkPane()
    {
    }
  };

  
} // namespace GUI

#endif //_MarkTableDelegat_H_C595B6B3_EF48_4F54_879C_E662D3876A5C_INCLUDED_

/* ===[ End of file  ]=== */
