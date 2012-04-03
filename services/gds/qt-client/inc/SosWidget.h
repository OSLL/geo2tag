#ifndef SOSWIDGET_H
#define SOSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "WriteTagQuery.h"
#include "Settings.h"
#include "LocationManager.h"

class SosWidget : public QWidget
{
    Q_OBJECT

    QPushButton *m_sosButton;
    WriteTagQuery *m_writeSosQuery;
    Settings m_settings;
    LocationManager *m_locationManager;

public:
    explicit SosWidget(LocationManager *locationManager, QWidget *parent = 0);

signals:

public slots:
    void sos();
    void onSosSent();
    void onError(QString error);


private:
    void initGUI();
};

#endif // SOSWIDGET_H
