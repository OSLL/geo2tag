/*
 * Copyright 2010  OSLL osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
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
      return (*m_channels)[index]->getName().c_str(); 
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
