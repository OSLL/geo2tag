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

    void setModelData(QWidget *editor, QAbstractItemModel *model,  const QModelIndex &index) const
    {
        qDebug() << "setDATA";
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

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
    {
      const ChannelModel *model = qobject_cast<const ChannelModel*>(index.model());
      QCheckBox cb(model->getChannelName(index.row()),NULL);

      if(option.state & QStyle::State_Selected)
      {
      //  painter->fillRect(option.rect, option.palette.color(QPalette::Highlight));
        cb.setBackgroundRole(QPalette::Highlight);
        cb.setAutoFillBackground(true);
      }

      cb.resize(option.rect.size());

      QPixmap pm=QPixmap::grabWidget(&cb);

      painter->drawImage(option.rect, pm.toImage());
    }

   QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */,  const QModelIndex & index) const
   {
     const ChannelModel *model = qobject_cast<const ChannelModel*>(index.model());
     QString name = model->getChannelName(index.row());
     
     return new  QCheckBox(name,parent);
   }
  }; 
} // namespace GUI

#endif //_ChannelModel_H_DB57EF03_D76B_419E_A6AF_056745B0F2EA_INCLUDED_

/* ===[ End of file ]=== */
