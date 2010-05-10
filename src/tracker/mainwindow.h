#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QLabel>
#include "tracker.h"


class MainWindow : public QMainWindow
{
  Q_OBJECT;

  Settings   m_settings;
  QLabel    *m_status;
  QDateTime  m_lastUpdate;


public:
  MainWindow();

  void timerEvent(QTimerEvent *te);

protected:
  void initSettings();
  bool setMark();

public slots:

  void cleanLocalSettigns();
  void readSettings();
  void createSettings();

  void onApplyMarkResponse(QString);

};

#endif // MAINWINDOW_H
