#ifndef ABOUTWIDGET_H_
#define ABOUTWIDGET_H_

#include <QLabel>
#include <QWidget>
#include <QPushButton>

class AboutWidget : public QWidget
{
  Q_OBJECT
    QLabel *m_label;
  QPushButton *m_okButton;
  public:
    AboutWidget(QWidget *parent = 0);
  public slots:
    void onOkClicked();
    signals:
    void done();
};
#endif
