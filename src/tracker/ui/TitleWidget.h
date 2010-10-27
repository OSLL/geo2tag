#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class TitleWidget : public QWidget
{
    Q_OBJECT

    QLabel *m_status;
    QPushButton *m_startButton;
    QPushButton *m_stopButton;

public:
    TitleWidget(QWidget *parent = 0);

public slots:
    /* this widget should eliminate current status,
       it can be text or picture.
       In any case there should be methods to set
       this information */
    void setEnabled(bool enabled);
    void setStatus(QString status);
};

#endif // TITLEWIDGET_H
