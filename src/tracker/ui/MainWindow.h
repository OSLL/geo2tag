#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTcpSocket>
#include <QTextStream>

#include "ui_MainWindow.h"
#include "LogWidget.h"
#include "OptionsWidget.h"
#include "AboutWidget.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

    bool m_isServiceStarted;
  QTcpSocket *m_daemon;
  QTextStream *m_device;
  QString m_lastCoord;
  // contain data, recieved from daemon
  QString m_message;

  LogWidget * m_logWidget;
  OptionsWidget *m_optionsWidget;
  AboutWidget * m_aboutWidget;

  void updateState();

  private slots:
    void startButtonClicked();
    void logButtonClicked();
    void doneButtonClicked();
    void settingsButtonClicked();
    void aboutButtonClicked();

    void checkDaemon();
    void readData();

    void restartDaemon();
    void moveToFirstPage();

  public:
    explicit MainWindow(QWidget *parent = 0);

  protected:
    void changeEvent(QEvent *e);
};
// MAINWINDOW_H
#endif
