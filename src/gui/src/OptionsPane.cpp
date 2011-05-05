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
 * $Id$
 *
 * \file OptionsPane.cpp
 * \brief OptionsPane implementation
 *
 * File description
 *
 * PROJ: geo2tag
 * ---------------------------------------------------------------- */

#include "OptionsPane.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QNetworkProxy>
#include <QRegExpValidator>
#include <QPalette>

#ifndef DEFAULT_SOURCE_TYPE
#define DEFAULT_SOURCE_TYPE GOOGLE
#endif

namespace GUI
{
  OptionsPane::OptionsPane(QWidget *parent) : QScrollArea(parent)
  {
    QWidget *mainWidget = new QWidget(this);
    QPalette *m_palette = new QPalette(Qt::black);
    //setPalette(*m_palette);
    //this->setForegroundRole(QPalette::WindowText);

    /* Add maps source type options */

    m_sourceTypeBox = new QGroupBox(tr("Source of maps:"), mainWidget);
    m_googleButton = new QRadioButton(tr("Google maps"), m_sourceTypeBox);
    m_openStreetButton = new QRadioButton(tr("Open street map"), m_sourceTypeBox);

    if (maps::MapLoader::GOOGLE == maps::MapLoader::DEFAULT_SOURCE_TYPE)
      m_googleButton->setChecked(true);
    if (maps::MapLoader::OPEN_STREET == maps::MapLoader::DEFAULT_SOURCE_TYPE)
      m_openStreetButton->setChecked(true);

    connect(m_googleButton, SIGNAL(clicked()), this, SLOT(onGoogleButtonClicked()));
    connect(m_openStreetButton, SIGNAL(clicked()), this, SLOT(onOpenStreeButtonClicked()));

    //        QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::HttpProxy,
    //                                                         "192.168.0.251",
    //                                                         3128));

    QVBoxLayout *sourceTypeBoxLayout = new QVBoxLayout(m_sourceTypeBox);
    sourceTypeBoxLayout->addWidget(new QLabel(QString(" "), m_sourceTypeBox));
    sourceTypeBoxLayout->addWidget(m_googleButton);
    sourceTypeBoxLayout->addWidget(m_openStreetButton);
    sourceTypeBoxLayout->addStretch();
    m_sourceTypeBox->setLayout(sourceTypeBoxLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainWidget->setLayout(mainLayout);
    mainLayout->addWidget(m_sourceTypeBox);

    /* Add proxy options */
    m_proxyBox = new QGroupBox(tr("Use proxy server"), mainWidget);
    m_hostProxyEdit = new QLineEdit(mainWidget);
    m_portProxyEdit = new QLineEdit(mainWidget);
    QPalette pal = m_hostProxyEdit->palette();
    pal.setColor(m_hostProxyEdit->backgroundRole(), Qt::transparent);
    m_hostProxyEdit->setPalette(pal);
    pal.setColor(m_hostProxyEdit->backgroundRole(), Qt::black);
    m_portProxyEdit->setPalette(pal);

    //        m_hostProxyEdit->setBackgroundRole(QPalette::Dark);
    //        m_portProxyEdit->setForegroundRole(m_palette->Background);
    QRegExpValidator* ValIPAddr;
    // match the Bytes can be from 0-199 or 200-249 or 250-255 but not a number with one 0 at the beginning like 001 or 020
    QString Byte = "(?!0[0-9])(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp rxip;
    rxip.setPattern("^" + Byte + "\\." + Byte + "\\." + Byte + "\\." + Byte + "$");
    ValIPAddr = new QRegExpValidator(rxip, this);
    m_hostProxyEdit->setValidator(ValIPAddr);
    m_portProxyEdit->setValidator(new QIntValidator(0, 65536, this));
    m_proxyBox->setCheckable(true);
    m_proxyBox->setChecked(false);

    connect(m_proxyBox, SIGNAL(clicked()), this, SLOT(onProxyBoxClicked()));
    connect(m_hostProxyEdit, SIGNAL(editingFinished()), this, SLOT(onProxyBoxClicked()));
    connect(m_portProxyEdit, SIGNAL(editingFinished()), this, SLOT(onProxyBoxClicked()));

    QVBoxLayout *proxyLayout = new QVBoxLayout(m_proxyBox);
    proxyLayout->addWidget(new QLabel(QString(" "), mainWidget));
    proxyLayout->addWidget(m_hostProxyEdit);
    proxyLayout->addWidget(m_portProxyEdit);
    proxyLayout->addStretch();
    m_proxyBox->setLayout(proxyLayout);

    mainLayout->addWidget(m_proxyBox);
    //mainWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->setWidget(mainWidget);

  }

  OptionsPane::~OptionsPane()
  {

  }

  void OptionsPane::onGoogleButtonClicked()
  {
    if (m_googleButton->isChecked())
    {
      emit sourceTypeUpdated(maps::MapLoader::GOOGLE);
    }
  }

  void OptionsPane::onOpenStreeButtonClicked()
  {
    if (m_openStreetButton->isChecked())
    {
      emit sourceTypeUpdated(maps::MapLoader::OPEN_STREET);
    }
  }

  void OptionsPane::onProxyBoxClicked()
  {
    if (m_proxyBox->isChecked())
    {
      QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::HttpProxy,
        m_hostProxyEdit->text(),
        m_portProxyEdit->text().toInt()));
      qDebug() << "proxy " <<  m_hostProxyEdit->text() << ":"
        <<  m_portProxyEdit->text().toInt() << "set";
    }
    else
    {
      QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::NoProxy));
      qDebug() << "no proxy";
    }

  }
}                                       // namespace GUI


/* ===[ End of file $HeadURL$ ]=== */
