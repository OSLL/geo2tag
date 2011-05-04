#ifndef TRACKERGUI_H
#define TRACKERGUI_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QTimer>

#include "RequestSender.h"
#include "OptionsWidget.h"
#include "LogWidget.h"

class TrackerGUI : public QMainWindow
{
  Q_OBJECT

    RequestSender *m_requestSender;

  QStackedWidget *m_stackedWidget;
  QWidget *m_titleWidget;
  QPushButton *m_status;
  QPushButton *m_startButton;
  QPushButton *m_stopButton;
  QPushButton *m_optionsButton;
  QPushButton *m_logButton;
  OptionsWidget *m_optionsWidget;
  LogWidget *m_logWidget;

  QAction *m_startAction;
  QAction *m_stopAction;
  QAction *m_optionsAction;
  QAction *m_logAction;

  QTimer *m_statusTimer;

  public:
    TrackerGUI(QWidget *parent = 0);

  public slots:
    void onStartTriggered();
    void onStopTriggered();
    void switchTitle();
    void switchOptions();
    void switchLog();
    void onOptionsWidgetDone();
    void onLogWidgetDone();
    void connected();
    void updateStatus();
  private slots:
    void displayError(QAbstractSocket::SocketError socketError);
};
// TRACKERGUI_H
#endif
