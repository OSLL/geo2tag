/*! ---------------------------------------------------------------
 *  
 *
 * \file ChannelModel.cpp
 * \brief ChannelModel implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "ChannelModel.h"

namespace GUI
{
    ChannelModel::ChannelModel(CHandlePtr<common::Channels> channels, QObject* parent) : QStandardItemModel(channels->size(),1,parent)
    {
      m_channels = channels;
    }

    QString ChannelModel::getChannelName(int index) const
    {

      return QString("DBG: ChannelNAME???")+(*m_channels)[index]->getName().c_str(); 
    }

    QString ChannelModel::getChannelDescription(int index) const
    {
      return (*m_channels)[index]->getDescription().c_str(); 
    }

    bool ChannelModel::IsSelected(int index) const
    {
      return (*m_channels)[index]->isDisplayed();
    }

    void ChannelModel::setSelection(int index, bool value)
    {
      (*m_channels)[index]->setDisplayed(value); 
    }

    int ChannelModel::rowCount(const QModelIndex &/*parent = QModelIndex()*/) const
    {
        return m_channels->size();
    }
      
    int ChannelModel::columnCount ( const QModelIndex & /*parent = QModelIndex()*/ ) const
    {
        return 1;
    }
    
   QVariant ChannelModel::data(const QModelIndex &index, int role) const
   {
    QString value = "?";
   	if ( Qt::DisplayRole == role && index.column() == 0 )
    {
      value = getChannelName(index.row()); 
   		return value;
   	}

   	return QVariant();
   }

    Qt::ItemFlags ChannelModel::flags(const QModelIndex& index) const
    {
      return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
} // namespace GUI

/* ===[ End of file ]=== */
