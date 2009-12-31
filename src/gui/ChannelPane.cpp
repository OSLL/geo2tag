/*! ---------------------------------------------------------------
 *  
 *
 * \file ChannelPane.cpp
 * \brief ChannelPane implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include <QtGui/QWidget>
#include <QtGui/QItemDelegate>
#include <QAbstractItemModel>
#include <QCheckBox>
#include <QStandardItemModel>
#include <QMouseEvent>
#include <QtGui/QListView>
#include "DbSession.h"
#include "ChannelPane.h"
#include "ChannelModel.h"


namespace GUI
{
  ChannelPane::ChannelPane(QWidget* parent) : QListView(parent)
  {    
    using namespace common;
    
    setModel(new ChannelModel(DbSession::getInstance().getChannels(),this));
    setItemDelegate( new ChannelDelegate() );

  }

  bool  ChannelPane::eventFilter(QObject *o, QEvent *e)
  {
    qDebug() << "Filter Event ";
    if (e->type() == QEvent::MouseButtonDblClick)
    {
        emit selectChannel();
        return true;
    }      
    else
    {
        return QWidget::eventFilter(o,e);
    }
  }
  
  void ChannelPane::selectChannel()
  {
//TODO: debug
      qDebug() << "channel event ";
  }

} // namespace GUI

/* ===[ End of file ]=== */
