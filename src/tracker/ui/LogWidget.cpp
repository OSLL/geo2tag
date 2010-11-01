#include "LogWidget.h"

#include "QVBoxLayout"

LogWidget::LogWidget(QWidget *parent) :
        QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_log = new QTextEdit());
    layout->addWidget(m_okButton = new QPushButton("Ok", this));
    this->setLayout(layout);

    connect(m_okButton, SIGNAL(clicked()), this, SLOT(onOkClicked()));
}

QString LogWidget::getLog()
{
    return m_log->toPlainText();
}

void LogWidget::setLog(QString log)
{
    m_log->setText(log);
}

void LogWidget::addToLog(QString string)
{
    m_log->setText(string + "\n \n" + m_log->toPlainText());
}

void LogWidget::onOkClicked()
{
    emit done();
}

