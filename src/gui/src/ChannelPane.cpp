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
#include "ChannelPane.h"
#include "ChannelModel.h"
#include <QLayout>
#include <QPushButton>

#include "OnLineInformation.h"

namespace GUI
{
  ChannelPane::ChannelPane(QWidget* parent) : QWidget(parent)
  {
    listView = new QListView(this);
    subscribeButton = new QPushButton(tr("Subscribe/unsubscribe"), this);
    tagsButton = new QPushButton(tr("Channel's tags"), this);

    connect(&OnLineInformation::getInstance(), SIGNAL(availableChannelsUpdated(QSharedPointer<Channels>)),
      this, SLOT(onChannelsUpdated()));
    connect(&OnLineInformation::getInstance(), SIGNAL(subscribedChannelsUpdated(QSharedPointer<Channels>)),
      this, SLOT(onChannelsUpdated()));
    connect(subscribeButton, SIGNAL(clicked()), this, SLOT(onSubscribeButtonClicked()));

    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(subscribeButton);
    buttonsLayout->addWidget(tagsButton);
    layout->addWidget(listView);
    layout->addLayout(buttonsLayout);

    update();
  }

  QListView* ChannelPane::getListView()
  {
    return listView;
  }

  QPushButton* ChannelPane::getTagsButton()
  {
    return tagsButton;
  }

  void ChannelPane::update()
  {
    OnLineInformation::getInstance().updateAvailableChannels();

  }
  void ChannelPane::onChannelsUpdated()
  {
    if (OnLineInformation::getInstance().getAvailableChannels() != 0)
    {

      if((listView->model() != 0) && (listView->currentIndex().isValid()))
      {
        selectedChannel = dynamic_cast<GUI::ChannelModel*>(listView->model())->getChannelName(listView->currentIndex().row());
        qDebug() << " ------> selected " << selectedChannel;
      }
      else
      {
        selectedChannel = QString("");
        qDebug() << " ------> selected " << selectedChannel;
      }

      ChannelModel *model = new ChannelModel(OnLineInformation::getInstance().getAvailableChannels(),
        OnLineInformation::getInstance().getSubscribedChannels(),
        this);

      int selected = -1;
      for (int i = 0; i < model->rowCount(); i++)
      {
        qDebug() << " ------> " << i << " "
          << dynamic_cast<GUI::ChannelModel*>(model)->getChannelName(i);
        if (dynamic_cast<GUI::ChannelModel*>(model)->getChannelName(i) == selectedChannel)
        {
          selected = i;
          break;
        }
      }

      listView->setModel(model);
      listView->setCurrentIndex(model->index(selected, 0, QModelIndex()));

      if((listView->model() != 0) && (listView->currentIndex().isValid()))
      {
        qDebug() << " ------> valid " << selectedChannel;
      }
    }
  }

  void ChannelPane::onSubscribeButtonClicked()
  {
    if((listView->model() != 0) && (listView->currentIndex().isValid()))
    {
      QString channelName = dynamic_cast<GUI::ChannelModel*>(listView->model())->getChannelName(listView->currentIndex().row());
      if (!dynamic_cast<GUI::ChannelModel*>(listView->model())->isSubscribed(listView->currentIndex()))
      {
        qDebug() << "subscribing to channel " << channelName;
        OnLineInformation::getInstance().subscribeChannel(channelName);
      }
      else
      {
        qDebug() << "unsubscribing from channel " << channelName;
        OnLineInformation::getInstance().unsubscribeChannel(channelName);
      }
    }
  }
}                                       // namespace GUI


/* ===[ End of file ]=== */
