#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include "QWidget"
#include "QTextEdit"
#include "QPushButton"

class LogWidget : public QWidget
{
  Q_OBJECT

    QTextEdit *m_log;
  QPushButton *m_okButton;

  public:
    LogWidget(QWidget *parent = 0);
    QString getLog();
    void setLog(QString log);
    void addToLog(QString string);

  public slots:
    void onOkClicked();

    signals:
    void done();

    };
#endif                                                      // LOGWIDGET_H
