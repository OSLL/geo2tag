#ifndef TRACKINGWIDGET_H
#define TRACKINGWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPointF>
#include <QTextEdit>
#include <QThread>

#include "TrackingService.h"
#include "Settings.h"
#include "LocationManager.h"

class TrackingWidget : public QWidget
{
    Q_OBJECT

    QPushButton *m_trackingButton;
    QLabel *m_status;
    QTextEdit *m_log;

    LocationManager *m_locationManager;
    Settings m_settings;
    QThread m_trackingThread;
    TrackingService m_trackingService;

public:
    explicit TrackingWidget(LocationManager *locationManager, QWidget *parent = 0);

    bool isTracking() const;

signals:

public slots:
    void startTracking();
    void stopTracking();
    void changeTrackingMode();
    void checkTrackingMode();
    void appendToLog(QString message);
    void onMarkSent(QPointF coords);
    void onError(QString error);

private:
    void initGUI();


};

#endif // TRACKINGWIDGET_H
