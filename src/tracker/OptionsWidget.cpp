#include "OptionsWidget.h"

#include <QVBoxLayout>
#include <QLabel>

OptionsWidget::OptionsWidget(QWidget *parent) :
        QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel("Login:"));
    layout->addWidget(m_name = new QLineEdit());
    layout->addWidget(new QLabel("Password:"));
    layout->addWidget(m_password = new QLineEdit());
    layout->addWidget(new QLabel("Channel name:"));
    layout->addWidget(m_channel = new QLineEdit());
    layout->addStretch();
    layout->addWidget(m_doneButton = new QPushButton("Done"));
    this->setLayout(layout);

    connect(m_doneButton, SIGNAL(clicked()), this, SLOT(onDoneClicked()));

}

QString OptionsWidget::name()
{
    return m_name->text();
}

QString OptionsWidget::password()
{
    return m_password->text();
}

QString OptionsWidget::channel()
{
    return m_channel->text();
}

void OptionsWidget::onDoneClicked()
{
    emit this->done();
}
