#include "inc/SettingsWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent)
{
    m_serverUrlEdit = new QLineEdit(this);
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
    mainLayout->addWidget(new QLabel("Server URL", this));
    mainLayout->addWidget(m_serverUrlEdit);
    QHBoxLayout *btnsLayout = new QHBoxLayout();
    btnsLayout->addWidget(m_okButton);
    btnsLayout->addWidget(m_cancelButton);
    mainLayout->addLayout(btnsLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

void SettingsWidget::fill()
{
    m_serverUrlEdit->setText(m_settings.getServerUrl());
}

void SettingsWidget::onOkClicked()
{
    m_settings.setServerUrl(m_serverUrlEdit->text());
    emit saved();
}

void SettingsWidget::onCancelClicked()
{
    emit cancelled();
}


