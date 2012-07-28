#include "inc/SettingsWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QIntValidator>
SettingsWidget::SettingsWidget(QWidget *parent) :
QWidget(parent)
{
  m_description = new QTextEdit(this);
  m_serverUrlEdit = new QLineEdit(this);
  m_radiusEdit = new QLineEdit(this);
  m_radiusEdit->setValidator(new QIntValidator(0, 40000, this));
  m_trackingPeriodEdit = new QLineEdit(this);
  m_trackingPeriodEdit->setValidator(new QIntValidator(1, 1000, this));
  m_okButton = new QPushButton("Ok", this);
  m_cancelButton = new QPushButton("Cancel", this);

  initGUI();

  connect(m_okButton, SIGNAL(clicked()),
    this, SLOT(onOkClicked()));
  connect(m_cancelButton, SIGNAL(clicked()),
    this, SLOT(onCancelClicked()));
}


void SettingsWidget::initGUI()
{
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(new QLabel("User description:", this));
  //m_description->setMinimumHeight();
  mainLayout->addWidget(m_description);
  mainLayout->addWidget(new QLabel("Server URL", this));
  mainLayout->addWidget(m_serverUrlEdit);
  mainLayout->addWidget(new QLabel("Radius of events (km):", this));
  mainLayout->addWidget(m_radiusEdit);
  mainLayout->addWidget(new QLabel("Tracking period (min):", this));
  mainLayout->addWidget(m_trackingPeriodEdit);
  QHBoxLayout *btnsLayout = new QHBoxLayout();
  btnsLayout->addWidget(m_okButton);
  btnsLayout->addWidget(m_cancelButton);
  mainLayout->addLayout(btnsLayout);
  mainLayout->addStretch();
  setLayout(mainLayout);
}


void SettingsWidget::fill()
{
  m_description->setText(m_settings.getDescription());
  m_serverUrlEdit->setText(m_settings.getServerUrl());
  m_radiusEdit->setText(QString::number(m_settings.getRadius()));
  m_trackingPeriodEdit->setText(QString::number(m_settings.getTrackingPeriod()));
}


void SettingsWidget::onOkClicked()
{
  m_settings.setDescription(m_description->toPlainText());
  m_settings.setServerUrl(m_serverUrlEdit->text());
  m_settings.setRadius(m_radiusEdit->text().toInt());
  m_settings.setTrackingPeriod(m_trackingPeriodEdit->text().toInt());
  emit saved();
}


void SettingsWidget::onCancelClicked()
{
  emit cancelled();
}
