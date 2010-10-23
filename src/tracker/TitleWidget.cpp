#include "TitleWidget.h"

#include <QVBoxLayout>

TitleWidget::TitleWidget(QWidget *parent) :
        QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_status = new QLabel("Undefined"));
    this->setLayout(layout);
}

void TitleWidget::setEnabled(bool enabled)
{
    if (enabled)
    {
        m_status->setText("Enabled");
    }
    else
    {
        m_status->setText("Disabled");
    }
}

void TitleWidget::setStatus(QString status)
{
    m_status->setText(status);
}
