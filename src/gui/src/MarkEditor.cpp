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
 * \file MarkEditor.cpp
 * \brief MarkEditor implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */
#include <sstream>
#include "MarkEditor.h"
#include <QDebug>
#include <QMessageBox>
#include <QtGui/QVBoxLayout>
#include "DataMarks.h"
#include "Channel.h"
#include "GpsInfo.h"
#include <QDateTime>

namespace GUI
{
  MarkEditor::MarkEditor(QWidget *parent) : QWidget(parent)
  {
    m_ok = new QPushButton("Add tag", this);
    m_combo = new QComboBox(this);
    m_text = new QTextEdit("Enter new tag",this);
    m_text->selectAll();

    connect(m_ok, SIGNAL(pressed()), this, SLOT(applyMark()));
    connect(&OnLineInformation::getInstance(),
      SIGNAL(subscribedChannelsUpdated(QSharedPointer<Channels>)),
      this,
      SLOT(onSubscribedChannelsUpdated(QSharedPointer<Channels>)));
    connect(&OnLineInformation::getInstance(), SIGNAL(markApplied(int)),
      this, SLOT(onMarkApplied(int)));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_combo);
    layout->addWidget(m_text);
    layout->addWidget(m_ok);
    setLayout(layout);

    update();
  }

  void MarkEditor::update()
  {
    OnLineInformation::getInstance().updateSubscribedChannels();
  }                                     /* Request done. If response received: */

  void MarkEditor::onSubscribedChannelsUpdated(QSharedPointer<Channels> channels)
  {
    m_combo->clear();
    int i = 0;
    for(; i<channels->size(); i++)
    {
      m_combo->insertItem(i,QObject::tr(channels->at(i)->getName().toStdString().c_str()));
    }
  }

  static std::string genearateNextLabel()
  {
    size_t size = OnLineInformation::getInstance().getMarks()->size();
    // \ToDo look at me
    char label = (int)'A' + (size + 1) % 23;
    std::ostringstream s;
    s << label;
    return s.str();
  }

  void MarkEditor::applyMark()
  {
    /* New mark sending */
    QString text = m_text->toPlainText();
    qDebug() << "sending new mark " << text;
    qDebug() << "channel " << m_combo->currentText();
    OnLineInformation::getInstance().applyMark(m_combo->currentText(),
      QString(genearateNextLabel().c_str()),
      QString("http://www.unf.edu/groups/volctr/images/question-mark.jpg") /* unknown/undefined url*/,
      text);
  }                                     /* Request done. If response received: */

  void MarkEditor::onMarkApplied(int status)
  {
    if (status)
    {
      QMessageBox::information(this, QObject::tr("Information"), QObject::tr("Your tag saved"));
    }
    else
    {
      QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Error during save your tag."));
    }
  }

}                                       // namespace GUI


/* ===[ End of file ]=== */
