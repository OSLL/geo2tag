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

    QSharedPointer<Channels> m_availableChannels;
    QSharedPointer<Channels> m_subscribedChannels;

    public:

      ChannelModel(QSharedPointer<Channels> availableChannels,
        QSharedPointer<Channels> subscribedChannels,
        QObject* parent);

      QString getChannelName(int index) const;
      QString getChannelDescription(int index) const;
      bool IsSelected(int index) const;
      void setSelection(int index, bool value);
      int rowCount(const QModelIndex & = QModelIndex()) const;
      int columnCount ( const QModelIndex & /*parent = QModelIndex()*/ ) const;
      QVariant data(const QModelIndex &index, int role) const;
      int isSubscribed(const QModelIndex& index) const;

      Qt::ItemFlags flags(const QModelIndex& index) const;

    private:
      ChannelModel(const ChannelModel& obj);
      ChannelModel& operator=(const ChannelModel& obj);

      // class ChannelModel
  };

  // namespace GUI
}


//_ChannelModel_H_DB57EF03_D76B_419E_A6AF_056745B0F2EA_INCLUDED_
#endif

/* ===[ End of file ]=== */
