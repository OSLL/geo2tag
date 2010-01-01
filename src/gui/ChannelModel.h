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


    ChannelModel(CHandlePtr<common::Channels> channels, QObject* parent);

    QString getChannelName(int index) const;
    QString getChannelDescription(int index) const;
    bool IsSelected(int index) const;
    void setSelection(int index, bool value);
    int rowCount(const QModelIndex &/*parent = QModelIndex()*/) const;
    int columnCount ( const QModelIndex & /*parent = QModelIndex()*/ ) const;
    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex& index) const;

  private:    
    ChannelModel(const ChannelModel& obj);
    ChannelModel& operator=(const ChannelModel& obj);

  }; // class ChannelModel
  
} // namespace GUI

#endif //_ChannelModel_H_DB57EF03_D76B_419E_A6AF_056745B0F2EA_INCLUDED_

/* ===[ End of file ]=== */
