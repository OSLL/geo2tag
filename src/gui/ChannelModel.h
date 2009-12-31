/*!
 * \file ChannelModel.h
 * \brief Header of ChannelModel
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _ChannelModel_H_DB57EF03_D76B_419E_A6AF_056745B0F2EA_INCLUDED_
#define _ChannelModel_H_DB57EF03_D76B_419E_A6AF_056745B0F2EA_INCLUDED_

#include <QStandardItemModel>
#include <QCheckBox>
#include <QItemDelegate>
#include <QPainter>
#include <QDebug>
#include "Channel.h"

namespace GUI
{
 /*!
   * Class description. May use HTML formatting
   *
   */
  class ChannelModel : public QStandardItemModel
  {

    Q_OBJECT;
    
    CHandlePtr<common::Channels>   m_channels;

  public:


    ChannelModel(CHandlePtr<common::Channels> channels, QObject* parent) : QStandardItemModel(channels->size(),1,parent)
    {
      m_channels = channels;
    }

    QString getChannelName(int index) const
    {
      return (*m_channels)[index]->getDescription().c_str(); 
    }

    bool IsSelected(int index) const
    {
      return (*m_channels)[index]->isDisplayed();
    }

    void setSelection(int index, bool value)
    {
      (*m_channels)[index]->setDisplayed(value); 
    }

    int rowCount(const QModelIndex &/*parent = QModelIndex()*/) const
    {
        return m_channels->size();
    }
      
    int columnCount ( const QModelIndex & /*parent = QModelIndex()*/ ) const
    {
        return 1;
    }
    
   QVariant data(const QModelIndex &index, int role) const
   {

    qDebug() << "model:data";
    QString value = "?";
   	if ( Qt::DisplayRole == role && index.column() == 0 )
    {
      value = getChannelName(index.row()); 
   		return value;
   	}

   	return QVariant();
   }

    Qt::ItemFlags flags(const QModelIndex& index) const
    {
      return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }


  private:    
    ChannelModel(const ChannelModel& obj);
    ChannelModel& operator=(const ChannelModel& obj);

  }; // class ChannelModel
  
 /*!
   * Class description. May use HTML formatting
   *
   */
  class ChannelDelegate : public QItemDelegate
  {
     Q_OBJECT;
  public:
    ChannelDelegate(QWidget *parent =NULL) : QItemDelegate(parent)
    {
    }

    int rowCount(const QModelIndex &/*parent = QModelIndex()*/) const
    {
      qDebug() << "del";
        return 5;
    }
      
    int columnCount ( const QModelIndex & /*parent = QModelIndex()*/ ) const
    {
        return 1;
    }

  }; 
} // namespace GUI

#endif //_ChannelModel_H_DB57EF03_D76B_419E_A6AF_056745B0F2EA_INCLUDED_

/* ===[ End of file ]=== */
