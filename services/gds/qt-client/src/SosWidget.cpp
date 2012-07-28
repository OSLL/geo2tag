/*
 * Copyright 2012  Ivan Bezyazychnyy  ivan.bezyazychnyy@gmail.com
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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

#include "inc/SosWidget.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QIcon>
#include "DataMarks.h"
#include "User.h"
#include "Channel.h"
#include "defines.h"
#include "JsonUser.h"

SosWidget::SosWidget(LocationManager *locationManager, QWidget *parent) :
QWidget(parent),
m_locationManager(locationManager)
{
  m_sosButton = new QPushButton();
  QPixmap m_sosPixmap(":/data/sos128.png");
  QIcon icon(m_sosPixmap);
  QSize iconSize(m_sosPixmap.width(), m_sosPixmap.height());
  m_sosButton->setIconSize(iconSize);
  m_sosButton->setIcon(icon);
  //m_sosButton = new QPushButton(QIcon(":/data/sos128.png"), this);

  initGUI();

  m_writeSosQuery = new WriteTagQuery(this);

  connect(m_sosButton, SIGNAL(clicked()), this, SLOT(sos()));
  connect(m_writeSosQuery, SIGNAL(tagAdded()),
    this, SLOT(onSosSent()));
  connect(m_writeSosQuery, SIGNAL(errorOccured(QString)),
    this, SLOT(onError(QString)));
}


void SosWidget::initGUI()
{
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(m_sosButton, 0, Qt::AlignVCenter);
  //mainLayout->addStretch();
  this->setLayout(mainLayout);
}


void SosWidget::sos()
{
  qDebug() << "sos clicked";
  QSharedPointer<DataMark> sosMark;
  QString description = m_settings.getDescription();
  if (description.isEmpty())
    description = "no description";
  sosMark = QSharedPointer<DataMark>(
    new DataMark(0, 0, 0, "SOS", description, "", QDateTime::currentDateTime()));

  QGeoPositionInfo info = m_locationManager->getInfo();
  if (info.isValid())
  {
    QSharedPointer<Channel> channel;
    QSharedPointer<common::User> user;
    channel = QSharedPointer<Channel>(new Channel(EVENTS_CHANNEL, ""));
    user = QSharedPointer<JsonUser>(new JsonUser(m_settings.getLogin(),
      m_settings.getPassword(),
      m_settings.getAuthToken()));
    sosMark->setUser(user);
    sosMark->setChannel(channel);
    sosMark->setLatitude(info.coordinate().latitude());
    sosMark->setLongitude(info.coordinate().longitude());
    sosMark->setTime();
    m_writeSosQuery->setTag(sosMark);
    m_writeSosQuery->setUrl(m_settings.getServerUrl());
    m_writeSosQuery->doRequest();
  }
  else
  {
    qDebug() << "invalid geo info, waiting and trying again";
    QTimer::singleShot(DEFAULT_SOS_PERIOD * 1000, this, SLOT(sos()));
  }
}


void SosWidget::onSosSent()
{
  qDebug() << "Sos sent successfully!";
  QMessageBox::information(this, "GeoDoctorSearch", "SOS sent successfully");
}


void SosWidget::onError(QString error)
{
  qDebug() << "error occured during sos, error: " << error;
  // TODO: add to SOS status!
  QTimer::singleShot(DEFAULT_SOS_PERIOD * 1000, this, SLOT(sos()));
}
