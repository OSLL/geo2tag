#include "TitleWidget.h"

#include <QVBoxLayout>
#include <QPushButton>

TitleWidget::TitleWidget(QWidget *parent) :
        QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_status = new QLabel("Undefined"));
    QPushButton *m_startButton = new QPushButton(this);
    m_startButton->setIcon(QIcon(":/images/start.png"));
    m_startButton->setFlat(true);
    layout->addWidget(m_startButton);
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
