#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include "Settings.h"
#include "SosWidget.h"
#include "TrackingWidget.h"
#include "LocationManager.h"
#include "EventsWidget.h"

class MainWidget : public QTabWidget
{
    Q_OBJECT

    Settings m_settings;

    LocationManager *m_locationManager;
    SosWidget *m_sosWidget;
    EventsWidget *m_eventsWidget;
    TrackingWidget *m_trackingWidget;
    bool m_shouldStartTracking;

public:
    explicit MainWidget(QWidget *parent = 0);

signals:

public slots:
    void signIn(const QString& authToken);
    void signOut();

private:
    void initGUI();

};

#endif // MAINWIDGET_H
