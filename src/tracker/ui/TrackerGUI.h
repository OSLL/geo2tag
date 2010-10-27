#ifndef TRACKERGUI_H
#define TRACKERGUI_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>

#include "RequestSender.h"
#include "TitleWidget.h"
#include "OptionsWidget.h"
#include "LogWidget.h"

class TrackerGUI : public QMainWindow
{
    Q_OBJECT

    RequestSender *m_requestSender;

    QStackedWidget *m_stackedWidget;
    QWidget *m_titleWidget;
    QLabel *m_status;
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

#endif // TRACKERGUI_H
