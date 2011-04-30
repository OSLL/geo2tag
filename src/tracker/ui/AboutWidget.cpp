#include "AboutWidget.h"
#include <QVBoxLayout>


AboutWidget::AboutWidget(QWidget *parent) :
        QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_label = new QLabel());
    layout->addWidget(m_okButton = new QPushButton("Ok", this));
    this->setLayout(layout);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setText(tr("Wikigps-tracker 0.1.5\n(c) 2010-2011 OSLL"));
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(onOkClicked()));
}

void AboutWidget::onOkClicked()
{
    emit done();
}
