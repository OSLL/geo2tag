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

#ifndef TRACKINGWIDGET_H
#define TRACKINGWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPointF>
#include <QTextEdit>
#include <QThread>

#include "TrackingService.h"
#include "Settings.h"
#include "LocationManager.h"

class TrackingWidget : public QWidget
{
  Q_OBJECT

    QPushButton *m_trackingButton;
  QLabel *m_status;
  QTextEdit *m_log;

  LocationManager *m_locationManager;
  Settings m_settings;
  QThread m_trackingThread;
  TrackingService m_trackingService;

  public:
    explicit TrackingWidget(LocationManager *locationManager, QWidget *parent = 0);

    bool isTracking() const;

    signals:

  public slots:
    void startTracking();
    void stopTracking();
    void changeTrackingMode();
    void checkTrackingMode();
    void appendToLog(QString message);
    void onMarkSent(QPointF coords);
    void onError(QString error);
    void onSettingsUpdated();

  private:
    void initGUI();

};
#endif                                  // TRACKINGWIDGET_H
