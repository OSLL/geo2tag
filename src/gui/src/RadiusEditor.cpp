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
#include "RadiusEditor.h"

namespace GUI
{
  RadiusEditor::RadiusEditor(QWidget *parent, QSharedPointer<Channel> channel):QDialog(parent), m_channel(channel)
  {
    setWindowTitle(QString("Editing radius for channel: ") + QString(channel->getName()));
    QVBoxLayout *vbox = new QVBoxLayout();
    m_value = new QSpinBox();
    m_value->setMinimum(200);
    m_value->setMaximum(15000);
    m_value->setValue(m_channel->getRadius()*1000);
    m_value->setSingleStep(200);
    QHBoxLayout *hbox = new QHBoxLayout();
    QLabel *label = new QLabel("Radius: ");
    QLabel *met = new QLabel(" metres");
    hbox->addWidget(label);
    hbox->addWidget(m_value);
    hbox->addWidget(met);
    hbox->addStretch();
    QDialogButtonBox *bbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    vbox->addLayout(hbox);
    vbox->addWidget(bbox);
    setLayout(vbox);
    connect(bbox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(bbox, SIGNAL(rejected()), this, SLOT(reject()));
  }
  void RadiusEditor::accept()
  {
    m_channel->setRadius(m_value->value()/1000.);
    QDialog::accept();
  }
}
