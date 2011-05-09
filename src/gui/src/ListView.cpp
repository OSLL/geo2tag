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
#include "ListView.h"
#include "OnLineInformation.h"
#include <QDebug>

namespace GUI
{
  ListView::ListView(QWidget *parent) : QTableView(parent)
  {
    m_model = new ListModel(this);
    setModel(m_model);
    m_model->setHeaderData(0, Qt::Horizontal, tr("Map"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("Tag text"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("Author"));

    connect(&OnLineInformation::getInstance(), SIGNAL(marksUpdated(QSharedPointer<DataMarks>)),
      (ListModel*)m_model, SLOT(marksUp(QSharedPointer<DataMarks>)));

    setItemDelegate(new MarkTableDelegat(this));
    horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
    verticalHeader()->hide();
  }

  void ListView::updateModel()
  {
    qDebug() << "update Model";
    //m_model->layoutUpdate();
    //qobject_cast<ListModel*>(m_model)->layoutUpdate();
  }
}
