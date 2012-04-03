#include "inc/SosWidget.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include "DataMarks.h"
#include "User.h"
#include "Channel.h"
#include "defines.h"
#include "JsonUser.h"

SosWidget::SosWidget(LocationManager *locationManager, QWidget *parent) :
    QWidget(parent),
    m_locationManager(locationManager)
{
    m_sosButton = new QPushButton("SOS", this);

    initGUI();

    m_writeSosQuery = new WriteTagQuery(this);

    connect(m_sosButton, SIGNAL(clicked()), this, SLOT(sos()));
    connect(m_writeSosQuery, SIGNAL(tagAdded()),
            this, SLOT(onSosSent()));
    connect(m_writeSosQuery, SIGNAL(errorOccured(QString)),
            this, SLOT(onError(QString)));
}

void SosWidget::initGUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_sosButton);
    mainLayout->addStretch();
    this->setLayout(mainLayout);
}

void SosWidget::sos()
{
    qDebug() << "sos clicked";
    QSharedPointer<DataMark> sosMark;
    sosMark = QSharedPointer<DataMark>(
               new DataMark(0, 0, 0, "SOS", "SOS", "", QDateTime::currentDateTime()));

    QGeoPositionInfo info = m_locationManager->getInfo();
    if (info.isValid()) {
        QSharedPointer<Channel> channel;
        QSharedPointer<common::User> user;
        channel = QSharedPointer<Channel>(new Channel(EVENTS_CHANNEL, ""));
        user = QSharedPointer<JsonUser>(new JsonUser(m_settings.getLogin(),
                                                     m_settings.getPassword(),
                                                     m_settings.getAuthToken()));
        sosMark->setUser(user);
        sosMark->setChannel(channel);
        sosMark->setLatitude(info.coordinate().latitude());
        sosMark->setLongitude(info.coordinate().longitude());
        sosMark->setTime();
        m_writeSosQuery->setTag(sosMark);
        m_writeSosQuery->setUrl(m_settings.getServerUrl());
        m_writeSosQuery->doRequest();
    } else {
        qDebug() << "invalid geo info, waiting and trying again";
        QTimer::singleShot(3 * 1000, this, SLOT(sos()));
    }
}

void SosWidget::onSosSent()
{
    qDebug() << "Sos sent successfully!";
    QMessageBox::information(this, "GeoDoctorSearch", "SOS sent successfully");
}

void SosWidget::onError(QString error)
{
    qDebug() << "error occured during sos, error: " << error;
    // TODO: add to SOS status!
    QTimer::singleShot(3 * 1000, this, SLOT(sos()));
}


