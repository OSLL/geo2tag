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

#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QSpinBox>
#include <QComboBox>
#include <QSettings>
#include "defines.h"

class OptionsWidget : public QScrollArea
{
  Q_OBJECT

    QWidget * m_widget;

  QLineEdit *m_nameEdit;
  QLineEdit *m_passwordEdit;
  QLineEdit *m_visibleNameEdit;
  QLineEdit *m_channelEdit;
  QLineEdit *m_proxyHostEdit;
  QLineEdit *m_serverUrlEdit;
  QSpinBox *m_serverPortEdit;

  QCheckBox * m_passwordCheckBox;

  QSpinBox * m_proxyPortEdit;
  QComboBox * m_proxyType;

  QComboBox * m_cacheType;
  QLineEdit * m_cachePath;
  QSpinBox * m_cacheMaxSize;

  QString m_name;
  QString m_password;
  QString m_channel;
  QString m_productName;

  QPushButton * m_cachePathButton;
  QPushButton * m_doneButton;
  QPushButton * m_cancelButton;
  QPushButton * m_defaultButton;

  QSettings m_settings;

  private:
    void applyProxySettings();

  public:
    OptionsWidget(QString productName,QWidget *parent = 0);

    QString name();
    QString password();
    QString channel();

    void initSettings();
    void readSettings();
    void createSettings();
    void updateWidgets();

  public slots:
    void onDoneClicked();
    void onCancelClicked();
    void setDefaultSettings();
    void onProxyTypeChanged(int index);
    void onCacheTypeChanged(int index);
    void onShowPasswordChecked(bool checked);
    void onCachePathButtonClick();
    void onCachePathSelected(QString path);

    signals:
    /* is emitted when "ok" button is pressed */
    void done();
    void cancel();
};
// OPTIONSWIDGET_H
#endif
