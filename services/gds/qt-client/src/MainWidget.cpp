#include "inc/MainWidget.h"
#include <QVBoxLayout>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    m_label = new QLabel("Hello, World!", this);

    initGUI();
}

void MainWidget::initGUI()
{
    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);
    layout->addWidget(m_label);
}

void MainWidget::signIn(const QString &authToken)
{
}

void MainWidget::signOut()
{
}
