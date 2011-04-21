#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTcpSocket>
#include <QTextStream>

#include "ui_MainWindow.h"
#include "LogWidget.h"
#include "OptionsWidget.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    bool m_isServiceStarted;
    QTcpSocket *m_daemon;
    QTextStream *m_device;
    QString m_lastCoord;
    QString m_message; // contain data, recieved from daemon

    LogWidget * m_logWidget;
    OptionsWidget *m_optionsWidget;

    void updateState();

private slots:
    void startButtonClicked();
    void logButtonClicked();
    void doneButtonClicked();
    void settingsButtonClicked();

    void checkDaemon();
    void readData();

    void moveToFirstPage();

public:
    explicit MainWindow(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
};

#endif // MAINWINDOW_H
