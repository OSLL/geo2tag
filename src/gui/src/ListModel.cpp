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
#include "ListModel.h"
#include "DataMarks.h"
#include "OnLineInformation.h"
#include <QDebug>

namespace GUI
{
  ListModel::ListModel(QObject* parent) :
  QStandardItemModel(OnLineInformation::getInstance().getMarks()->size(), 3, parent),
    m_data(OnLineInformation::getInstance().getMarks())
  {
    m_size = 0;
  }

  int ListModel::rowCount(const QModelIndex &/*parent = QModelIndex()*/) const
  {
    return m_size;
  }

  int ListModel::columnCount ( const QModelIndex & /*parent = QModelIndex()*/ ) const
  {
    return 3;
  }

  QSharedPointer<Channel> ListModel::getCurrentChannel() const
  {
    return m_currentChannel;
  }

  void ListModel::setDescription(int row, const std::string& data)
  {
    size_t c=0,i=0;
    for(; i<m_data->size(); i++)
    {
      if((*m_data)[i]->getChannel() == m_currentChannel)
        c++;
      if((c-1)==row)
        break;
    }

    (*m_data)[i]->setDescription(QString(data.c_str()));
  }

  QVariant ListModel::data(const QModelIndex &index, int role) const
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
          value = (*m_data)[i]->getLabel();
          break;

        case 1:
          value = (*m_data)[i]->getDescription();
          break;

        case 2:
          value = (*m_data)[i]->getUser()->getLogin();
          break;

        default:
          break;
      }

      return value;
    }
    return QVariant();
  }

  void ListModel::layoutUpdate(QSharedPointer<Channel> channel)
  {
    qDebug() << "current channel = " << m_currentChannelName;
    if(channel!=0)
    {
      m_currentChannel=channel;
      m_currentChannelName = (*channel).getName();
      //        qDebug() << m_currentChannelName;
    }
    else if (m_currentChannelName != "")
    {
      QSharedPointer<Channel> newChannelPtr = OnLineInformation::getInstance().findChannel(m_currentChannelName);
      if (newChannelPtr != 0)
      {
        m_currentChannel = newChannelPtr;
      }
    }

    double longitude = common::GpsInfo::getInstance().getLongitude();
    double latitude = common::GpsInfo::getInstance().getLatitude();
    size_t size=0;
    //m_data = OnLineInformation::getInstance().getMarks();
    for(size_t i=0; i<m_data->size(); i++)
    {
      QSharedPointer<DataMark> mark = (*m_data)[i];
      qDebug() << "radius = " << mark->getChannel()->getRadius()
        << "disctance=" << DataMark::getDistance(latitude, longitude,mark->getLatitude(), mark->getLongitude());
      if(mark->getChannel()              == m_currentChannel &&
        mark->getChannel()->getRadius()*100 >  DataMark::getDistance(latitude, longitude,mark->getLatitude(), mark->getLongitude()))
        size++;
    }
    m_size = size;
    qDebug() << "m_size=" << m_size << " from " << m_data->size();
    setRowCount(size);
    emit layoutChanged();
  }
  void ListModel::marksUp(QSharedPointer<DataMarks> m_marks)
  {
    qDebug() << "marksUp got new marks set";
    m_data=m_marks;
    layoutUpdate();
  }

}
