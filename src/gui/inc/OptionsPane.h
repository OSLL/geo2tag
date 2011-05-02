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

/* $Id$ */
/*!
 * \file OptionsPane.h
 * \brief Header of OptionsPane
 * \todo add comment here
 *
 * File description
 *
 * PROJ: geo2tag
 * ---------------------------------------------------------------- */

#ifndef _OptionsPane_H_92651A52_B5F8_41C4_96AC_484BA7C02996_INCLUDED_
#define _OptionsPane_H_92651A52_B5F8_41C4_96AC_484BA7C02996_INCLUDED_

#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>

#include "MapLoader.h"

namespace GUI
{
  /*!
   * Class description. May use HTML formatting
   *
   */
  class OptionsPane : public QScrollArea
  {
    Q_OBJECT

      QGroupBox *m_sourceTypeBox;
    QRadioButton *m_googleButton;
    QRadioButton *m_openStreetButton;

    QGroupBox *m_proxyBox;
    QLineEdit *m_hostProxyEdit;
    QLineEdit *m_portProxyEdit;

    public:
      OptionsPane(QWidget *parent = 0);
      ~OptionsPane();

      signals:
      void sourceTypeUpdated(maps::MapLoader::SourceType sourceType);

    private slots:
      void onGoogleButtonClicked();
      void onOpenStreeButtonClicked();
      void onProxyBoxClicked();

    private:
      //OptionsPane(const OptionsPane& obj);
      //OptionsPane& operator=(const OptionsPane& obj);

      // class OptionsPane
  };

  // namespace GUI
}


//_OptionsPane_H_92651A52_B5F8_41C4_96AC_484BA7C02996_INCLUDED_
#endif

/* ===[ End of file $HeadURL$ ]=== */
