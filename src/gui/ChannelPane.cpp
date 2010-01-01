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
  }
} // namespace GUI

/* ===[ End of file ]=== */
