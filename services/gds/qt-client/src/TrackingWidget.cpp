#include "inc/TrackingWidget.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QDebug>


TrackingWidget::TrackingWidget(LocationManager *locationManager, QWidget *parent) :
    QWidget(parent),
    m_locationManager(locationManager),
    m_trackingService(m_locationManager)
{

    m_trackingButton = new QPushButton("Start tracking", this);
    m_status = new QLabel("Tracking stopped", this);
    m_log = new QTextEdit(this);
    m_log->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_log->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_trackingService.moveToThread(&m_trackingThread);

    initGUI();

    connect(m_trackingButton, SIGNAL(clicked()), this, SLOT(changeTrackingMode()));
    connect(&m_trackingService, SIGNAL(markSent(QPointF)),
            this, SLOT(onMarkSent(QPointF)));
    connect(&m_trackingService, SIGNAL(errorOccured(QString)),
            this, SLOT(onError(QString)));
}

void TrackingWidget::initGUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_trackingButton, 0);
    mainLayout->addWidget(m_status, 0);
    //QScrollArea *scrollArea = new QScrollArea(this);
    //scrollArea->setWidget(m_log);
    mainLayout->addWidget(m_log, 1);
    this->setLayout(mainLayout);
}

bool TrackingWidget::isTracking() const
{
    return m_trackingThread.isRunning();
}

void TrackingWidget::startTracking()
{
    m_trackingThread.start();
    QMetaObject::invokeMethod(&m_trackingService, "startTracking", Qt::QueuedConnection,
                              Q_ARG(QString, m_settings.getLogin()),
                              Q_ARG(QString, m_settings.getPassword()),
                              Q_ARG(QString, m_settings.getAuthToken()),
                              Q_ARG(QString, m_settings.getServerUrl()));
    m_trackingButton->setText("Stop tracking");
}

void TrackingWidget::stopTracking()
{
    m_trackingThread.quit();
    m_trackingThread.wait();
    checkTrackingMode();
}

void TrackingWidget::changeTrackingMode()
{
    qDebug() << "changeTrackingMode clicked";
    if (!m_trackingThread.isRunning()) {
        startTracking();
    } else {
        stopTracking();
    }
}

void TrackingWidget::checkTrackingMode()
{
    if (!m_trackingThread.isRunning()) {
        m_trackingButton->setText("Start tracking");
    } else {
        m_trackingButton->setText("Stop tracking");
    }
}

void TrackingWidget::appendToLog(QString message)
{
    m_log->setText(m_log->toPlainText() + "\n" + message);
}

void TrackingWidget::onMarkSent(QPointF coordinates)
{
    appendToLog(QString("Sent coordinates: ") + QString::number(coordinates.x())
                + ", " + QString::number(coordinates.y()));
}

void TrackingWidget::onError(QString error)
{
    appendToLog(QString("Error: ") + error);
}


