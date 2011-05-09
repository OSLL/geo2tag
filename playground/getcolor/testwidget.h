#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>

class TestWidget : public QWidget
{
  Q_OBJECT
    public:
    explicit TestWidget(QWidget *parent = 0);
    virtual void paintEvent(QPaintEvent *pe);

    signals:

  public slots:

};
// TESTWIDGET_H
#endif
