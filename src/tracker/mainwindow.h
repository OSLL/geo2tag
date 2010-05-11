#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QLabel>
#include <QPointer>
#include "tracker.h"

// QtMobility API headers
// Location
#include <qgeopositioninfosource.h>
#include <qgeopositioninfo.h>

// QtMobility namespace
QTM_USE_NAMESPACE


class MainWindow : public QMainWindow
{
  Q_OBJECT;

  Settings   m_settings;
  QLabel    *m_status;
  QDateTime  m_lastUpdate;

  // Location data
  QPointer<QGeoPositionInfoSource> m_positionSource;
  QGeoPositionInfo m_positionInfo;


public:
  MainWindow();

  void timerEvent(QTimerEvent *te);

protected:
  void initSettings();
  bool setMark();

private:
    void startGps();

public slots:

  void cleanLocalSettigns();
  void readSettings();
  void createSettings();

  void onApplyMarkResponse(QString);
  void positionUpdated(QGeoPositionInfo gpsPos);

};

#endif // MAINWINDOW_H
