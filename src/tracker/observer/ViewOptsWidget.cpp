/*
 * Copyright 2010-2011  OSLL osll@osll.spb.ru
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
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "ViewOptsWidget.h"
#include <QLabel>
#include <QDebug>
ViewOptsWidget::ViewOptsWidget(QWidget *parent):QScrollArea(parent)
{
  m_widg=new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(m_widg);
  layout->addWidget(new QLabel("Number of marks, shown from each channel:"));
  layout->addWidget(m_marksCountEdit = new QSpinBox(this) );
  layout->addWidget(new QLabel("Max age of mark:"));
  layout->addWidget(m_timeFilterEdit = new QSpinBox(this) );
  m_marksCountEdit->setMinimum(1);
  m_marksCountEdit->setMaximum(10);
  m_timeFilterEdit->setMaximum(60);
  m_timeFilterEdit->setMinimum(1);
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(m_doneButton = new QPushButton("Done"));
  buttonLayout->addWidget(m_cancelButton = new QPushButton("Cancel"));
  layout->addLayout(buttonLayout);
  m_widg->setLayout(layout);
  this->setWidget(m_widg);
  this->setWidgetResizable(true);
  connect(m_doneButton, SIGNAL(clicked()), this, SLOT(onDoneClicked()));
  connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(onCancelClicked()));

  QSettings settings(QSettings::SystemScope,"osll","libs");
  /*    if (settings.value("timeLimit").toInt()>60 || settings.value("timeLimit").toInt()<1){
        m_timeFilterEdit->setValue(4);
      }else {*/
  m_timeFilterEdit->setValue(settings.value("timeLimit").toInt());
  /* }
  if (settings.value("marksCount").toInt()>10 || settings.value("marksCount").toInt()<1){
   m_marksCountEdit->setValue(10);
  }else {*/
  m_marksCountEdit->setValue(settings.value("marksCount").toInt());
  //    }
  qDebug() << "initing timeLimit as " << m_timeFilterEdit->value();
  qDebug() << "initing marksCount as " << m_marksCountEdit->value();

}


void ViewOptsWidget::onDoneClicked()
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  settings.setValue("timeLimit",m_timeFilterEdit->value());
  settings.setValue("marksCount",m_marksCountEdit->value());

  qDebug() << "setting timeLimit as " << m_timeFilterEdit->value();
  qDebug() << "setting marksCount as " << m_marksCountEdit->value();
  emit done();
}


int ViewOptsWidget::getTimeLimit() const
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  qDebug() << "current timeLimit " << settings.value("timeLimit").toInt();
  return settings.value("timeLimit").toInt();

}


void ViewOptsWidget::onCancelClicked()
{
  emit cancel();
}


int ViewOptsWidget::getMarksCount() const
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  qDebug() << "current marksCount " << settings.value("marksCount").toInt();
  return settings.value("marksCount").toInt();
}
