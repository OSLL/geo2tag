#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QLabel>

class TitleWidget : public QWidget
{
    Q_OBJECT

    QLabel *m_status;

public:
    TitleWidget(QWidget *parent = 0);

public slots:
    void setEnabled(bool enabled);
    void setStatus(QString status);
};

#endif // TITLEWIDGET_H
