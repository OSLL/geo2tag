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
 * \file MarkPane.cpp
 * \brief MarkPane implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include <iostream>
#include <QWebView>
#include <QUrl>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QSpinBox>
#include <QDialogButtonBox>
#include "MarkPane.h"
#include "MarkTableDelegat.h"
#include "MarkDetailsDialog.h"
#include "RadiusEditor.h"
#include "OnLineInformation.h"

#ifndef DEFAULT_RADIUS
#define DEFAULT_RADIUS 500
#endif

namespace GUI
{

  MarkPane::MarkPane(QWidget * parent) : QWidget(parent)
  {
    m_list = new ListView(this);

    connect(m_list, SIGNAL(clicked(const QModelIndex&)), this, SLOT(showUrl(const QModelIndex&)));

    QHBoxLayout *vbl = new QHBoxLayout(this);
    vbl->addWidget(m_list);
    setLayout(vbl);
  }

  void MarkPane::showUrl(const QModelIndex &index)
  {
    //TODO:: here's image dialog, please
    // QMessageBox::information(this,"MarkImage","IIMAGE DBG IMAGE DBG IMAGE DBG IMAGE DBG MAGE DBG ");
    // We are looking for real index of selected mark ( we store all marks and show only for current channel)
    QSharedPointer<DataMarks> marks = OnLineInformation::getInstance().getMarks();
    size_t c=0,i=0;
    for(; i<marks->size(); i++)
    {
      if((*marks)[i]->getChannel() == dynamic_cast<GUI::ListModel*>(m_list->model())->getCurrentChannel())
        c++;
      if((c-1)==index.row())
        break;
    }
    // now "i" is a real mark index
    // We should draw dialog with our mark.
    MarkDetailsDialog dialog(this, (*marks)[i]);
    dialog.exec();
  }

  void MarkPane::refresh(QSharedPointer<Channel> channel)
  {
    qDebug() << "refresh marks view";
    qobject_cast<ListModel*>((m_list)->model())->layoutUpdate(channel);
  }

  void MarkPane::updateCurrentChannelRadius()
  {
    RadiusEditor dialog(this, dynamic_cast<GUI::ListModel*>(m_list->model())->getCurrentChannel());
    dialog.exec();
    refresh(dynamic_cast<GUI::ListModel*>(m_list->model())->getCurrentChannel());
  }

}                                       // namespace GUI


/* ===[ End of file ]=== */
