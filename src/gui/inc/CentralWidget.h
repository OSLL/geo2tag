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
 * \file CentralWidget.h
 * \brief Header of CentralWidget
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _CentralWidget_H_044270F5_8D38_4F61_9F05_96359B3BD122_INCLUDED_
#define _CentralWidget_H_044270F5_8D38_4F61_9F05_96359B3BD122_INCLUDED_

#include <QtGui/QWidget>
#include <QtGui/QStackedLayout>
#include <QDebug>
#include "MarkPane.h"
#include "MapPane.h"
#include "ChannelPane.h"
#include "MarkEditor.h"
#include "OptionsPane.h"

namespace GUI
{
  /*!
   * Class description. May use HTML formatting
   *
   */
  class CentralWidget : public QWidget
  {
    Q_OBJECT;

    QStackedLayout *m_layout;
    MapPane        *m_mapView;
    MarkPane       *m_feedView;
    ChannelPane    *m_channelView;
    MarkEditor     *m_editor;
    QPalette       *m_palette;
    OptionsPane    *m_options;

    public:
      CentralWidget(QWidget *parent);

      virtual ~CentralWidget()
      {
      }

    public slots:
      void switchMap();
      void switchFeed(const QModelIndex& index);
      void switchChannel();
      void switchEditor();
      void setRadius();
      void switchOptions();
      void onViewTagsButtonClicked();

    private:
      CentralWidget(const CentralWidget& obj);
      CentralWidget& operator=(const CentralWidget& obj);

      // class CentralWidget
  };

  // namespace GUI
}


//_CentralWidget_H_044270F5_8D38_4F61_9F05_96359B3BD122_INCLUDED_
#endif

/* ===[ End of file ]=== */
