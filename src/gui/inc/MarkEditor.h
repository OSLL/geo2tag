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
 * \file MarkEditor.h
 * \brief Header of MarkEditor
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _MarkEditor_H_8C1685C8_8471_494E_A858_6FB0BC6AD16C_INCLUDED_
#define _MarkEditor_H_8C1685C8_8471_494E_A858_6FB0BC6AD16C_INCLUDED_

#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include <QtGui/QComboBox>
#include <QtGui/QTextEdit>
#include "OnLineInformation.h"

namespace GUI
{
  /*!
   * Class description. May use HTML formatting
   *
   */
  class MarkEditor : public QWidget
  {
    Q_OBJECT

      QPushButton  *m_ok;
    QComboBox    *m_combo;
    QTextEdit    *m_text;

    public:

      MarkEditor(QWidget *parent);
      virtual ~MarkEditor()
      {
      }

    public slots:
      void update();

    private slots:
      void applyMark();
      void onSubscribedChannelsUpdated(QSharedPointer<Channels> channels);
      void onMarkApplied(int status);

    private:
      MarkEditor(const MarkEditor& obj);
      MarkEditor& operator=(const MarkEditor& obj);

      // class MarkEditor
  };

  // namespace GUI
}


//_MarkEditor_H_8C1685C8_8471_494E_A858_6FB0BC6AD16C_INCLUDED_
#endif

/* ===[ End of file ]=== */
